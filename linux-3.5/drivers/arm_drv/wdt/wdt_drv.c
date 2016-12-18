#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/clk.h>
#include <linux/platform_device.h>
#include "regs-wdt.h"

#define WDT_DEFTIME	15 //默认喂狗间隔15秒

#define WDT_TYPE	'W'
#define WDT_START	_IO(WDT_TYPE, 1)
#define WDT_STOP	_IO(WDT_TYPE, 2)
#define WDT_FEED	_IO(WDT_TYPE, 3)
#define WDT_CHGTIME 	_IOW(WDT_TYPE, 4, int)
#define WDT_CHGMODE	_IOW(WDT_TYPE, 5, int)

#define MODE_RESET	1
#define MODE_IRQ	2

//由于只有一个设备，可以不使用私有结构体
static u32 wdt_base;
static u32 wdt_size;
static struct resource *wdt_res;
static void __iomem *vir_base;

static int wdt_irq;  //中断号
static int wdt_time; //喂狗间隔秒数
static int wdt_count; //喂狗间隔的计数
static int wdt_reset;  //1: reset; 0: irq

static struct clk *wdt_clk;
static spinlock_t wdt_lock;


/******************************
 * 喂狗(通过ioctl调用)
 ******************************/
static void wdt_keepalive(void)
{
	writel(wdt_count, vir_base + WTCNT);
}


/*******************
 * 停止看门狗
 *******************/
static void wdt_stop(void)
{
	u32 wtcon;

	wtcon = readl(vir_base + WTCON);
	wtcon &= ~(WTCON_ENABLE | WTCON_RSTEN);
	writel(wtcon, vir_base + WTCON);
}


/******************
 * 启动看门狗
 ******************/
static void wdt_start(void)
{
	u32 wtcon;

	//默认使用128分频
	wtcon = readl(vir_base + WTCON);
	wtcon |= WTCON_ENABLE | WTCON_DIV128;

	if (wdt_reset) {
		wtcon |= WTCON_RSTEN;
		wtcon &= ~WTCON_INTEN;
		printk("WDT: support reset\n");	
	} else {
		wtcon |= WTCON_INTEN;
		wtcon &= ~WTCON_RSTEN;
		printk("WDT: support interrupt\n");	
	}
	printk("WDT: start, wdt_count=%d, wtcon=0x%x\n", wdt_count, wtcon);
	writel(wdt_count, vir_base + WTDAT);
	writel(wdt_count, vir_base + WTCNT);
	udelay(10);
	writel(wtcon, vir_base + WTCON);
}


/***********************
 *以秒为单位设置喂狗间隔
 ***********************/
static int wdt_set_heartbeat(int timeout)
{
	u32 freq = clk_get_rate(wdt_clk); /*100MHz*/
	u32 count;
	u32 presc = 1;
	u32 wtcon;

	printk("abc4412-wdt clk freq: %d\n", freq);
	if (timeout < 1)
		return -EINVAL;

	//默认对PCLK进行128分频
	freq /= 128;
	count = timeout * freq;

	if (count >= 0x10000) {
		for (presc = 2; presc <= 0x100; presc++) {
			if ((count/presc) < 0x10000)
				break;
		}

		if ((count/presc) >= 0x10000) {
			printk("timeout %d too big\n", timeout);
			return -EINVAL;
		}
	}

	wdt_time = timeout;
	count /= presc;
	wdt_count = count;

	printk("WDT: timeout=%d, presc=%d, wdt_count=%d\n", wdt_time, presc, wdt_count);

	/* 更新WTCON和WTDAT */
	wtcon = readl(vir_base + WTCON);
	wtcon &= ~WTCON_PRESCALE_MASK;
	wtcon |= WTCON_PRESCALE(presc-1);
	writel(wtcon, vir_base + WTCON);

	writel(wdt_count, vir_base + WTDAT);
	writel(wdt_count, vir_base + WTCNT);

	return 0;
}

/**************************
 * 切换看门狗的响应模式
 **************************/
static int wdt_chgmode(u32 mode)
{
	u32 wtcon;

	wtcon = readl(vir_base+WTCON);
	if (mode == MODE_IRQ) {
		wtcon &= ~WTCON_RSTEN;
		wtcon |= WTCON_INTEN;
		wdt_reset = 0;
	}
	else if (mode == MODE_RESET) {
		wtcon &= ~WTCON_INTEN;
		wtcon |= WTCON_RSTEN;
		wdt_reset = 1;
	}
	else {
		printk("WDT: unsupport mode %d\n", mode);
		return -1;
	}
	writel(wtcon, vir_base+WTCON);

	return 0;
}



/**************************
 * /proc/proc_wdt文件的读函数
 **************************/
static int 
wdt_proc_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int wtcon, wtdat, wtcnt, ret = 0;
	unsigned long flags;

	spin_lock_irqsave(&wdt_lock, flags);
	wtcon = readl(vir_base + WTCON);
	wtcnt = readl(vir_base + WTCNT);
	wtdat = readl(vir_base + WTDAT);
	spin_unlock_irqrestore(&wdt_lock, flags);

	ret += sprintf(page+ret, "====WDT(irq %d)====\n", wdt_irq);
	ret += sprintf(page+ret, "watchdog %sactive, reset %sabled, interrupt %sabled\n", 
		 (wtcon & WTCON_ENABLE) ?  "" : "in",
		 (wtcon & WTCON_RSTEN) ? "en" : "dis",
		 (wtcon & WTCON_INTEN) ? "en" : "dis");
	ret += sprintf(page+ret, "Heartbeat = %ds; wdt_count = %d;\n", wdt_time, wdt_count);
	ret += sprintf(page+ret, "WTDAT = %d; WTCNT = %d\n", wtdat, wtcnt);

	return ret;
}


/***************
 * 看门狗中断
 ***************/
static irqreturn_t 
wdt_service(int irq, void *dev_id)
{
	//清除中断标志
	printk("I'm starving, you need to feed me\n");

	spin_lock(&wdt_lock);
	writel(0, vir_base + WTCLRINT);
	spin_unlock(&wdt_lock);

	return IRQ_HANDLED;
}


/****************************
 * fops-->unlocked_ioctl
*****************************/
static long 
wdt_ioctl(struct file *filp, unsigned int req, unsigned long arg)
{
	int ret = 0;
	unsigned long flags;

	spin_lock_irqsave(&wdt_lock, flags);
	switch (req) {
	case WDT_START: 
		wdt_start();
		break;
	case WDT_STOP:
		wdt_stop();
		break;
	case WDT_FEED:
		wdt_keepalive();
		break;
	case WDT_CHGTIME:
		ret = wdt_set_heartbeat(arg);
		break;
	case WDT_CHGMODE:
		ret = wdt_chgmode(arg);
		break;
	default:
		printk("Cannot support ioctl: 0x%x\n", req);
		ret = -1;
	}
	spin_unlock_irqrestore(&wdt_lock, flags);

	return ret;
}


static struct file_operations wdt_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = wdt_ioctl,
};

//name为/dev下的设备文件名
static struct miscdevice abc4412_wdt = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "wdt0",
	.fops	= &wdt_fops,
};


/**********************
 * platform_driver->probe
 **********************/
static int 
wdt_probe(struct platform_device *pdev)
{
	struct resource *res1, *res2;
	int ret, wtcon;

	//1.获得资源
	res1 = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	res2 = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res1 || !res2) {
		printk("WDT: Cannot get enough resources\n");
		return -1;
	}

	//2.准备访问WDT寄存器
	wdt_base = res1->start;
	wdt_size = res1->end-res1->start+1;
#if 0
	wdt_res = request_mem_region(wdt_base, wdt_size, "4412wdt-res");
	if (!wdt_res) {
		printk("WDT: request failed\n");
		return -EIO;
	}
#endif
	vir_base = ioremap(wdt_base, wdt_size);
	if (!vir_base) {
		printk("WDT: Cannot ioremap\n");
		ret = -EIO;
		goto err_map;
	}

	//3.注册中断处理函数
	wdt_irq = res2->start;
	ret = request_irq(wdt_irq, wdt_service, 0, pdev->name, vir_base);
	if (ret) {
		printk("WDT: Cannot request irq %d\n", wdt_irq);
		goto err_irq;
	}

	//4.使能时钟
	//所有通过芯片内部PLL提供的时钟都需要执行clk_enable
	wdt_clk = clk_get(&pdev->dev, "watchdog");
	if (IS_ERR(wdt_clk)) {
		printk("WDT: failed to get clock\n");
		ret = PTR_ERR(wdt_clk);
		goto err_clk;
	}
	clk_enable(wdt_clk);

	//5.创建proc文件
	create_proc_read_entry("proc_wdt", 0444, NULL, 
		wdt_proc_read, NULL);

	//6.初始化变量
	spin_lock_init(&wdt_lock);
#if defined(CONFIG_SUPPORT_RESET)
	wdt_reset = 1;
#else
	wdt_reset = 0;
#endif	

	//7.设置默认的喂狗间隔
	//如果PCLK为100MHZ，最大喂狗间隔为21秒
	ret = wdt_set_heartbeat(CONFIG_WDT_TIME);
	if (ret) {
		printk("WDT: %dS is too long, use default time %dS\n", CONFIG_WDT_TIME, WDT_DEFTIME);
		ret = wdt_set_heartbeat(WDT_DEFTIME);
	}

	//8.默认关闭看门狗
	wdt_stop();

	//9.注册miscdevice
	ret = misc_register(&abc4412_wdt);
	if (ret) {
		printk("WDT: cannot register miscdevice\n");
		goto err_misc;
	}

	//10.显示信息
	wtcon = readl(vir_base + WTCON);
	printk("WDT: %sactive, reset %sabled, irq %sabled\n",
		 (wtcon & WTCON_ENABLE) ?  "" : "in",
		 (wtcon & WTCON_RSTEN) ? "en" : "dis",
		 (wtcon & WTCON_INTEN) ? "en" : "dis");

	return 0;
err_misc:
	remove_proc_entry("proc_wdt", NULL);
	clk_disable(wdt_clk);
	clk_put(wdt_clk);
err_clk:
	free_irq(wdt_irq, vir_base);	
err_irq:
	iounmap(vir_base);
err_map:
	release_mem_region(wdt_base, wdt_size);
	return ret;
}


/***************************
 * platform_driver->remove
 ***************************/
static int 
wdt_remove(struct platform_device *pdev)
{
	free_irq(wdt_irq, vir_base);
	misc_deregister(&abc4412_wdt);
	remove_proc_entry("proc_wdt", NULL);

	clk_disable(wdt_clk);
	clk_put(wdt_clk);

	iounmap(vir_base);
	release_mem_region(wdt_base, wdt_size);

	return 0;
}

static struct platform_driver wdt_drv = {
	.probe = wdt_probe,
	.remove = wdt_remove,
	.driver = {
		.name = "abc4412-wdt",
	},
};

module_platform_driver(wdt_drv);

MODULE_AUTHOR("mill9527");
MODULE_LICENSE("GPL");

