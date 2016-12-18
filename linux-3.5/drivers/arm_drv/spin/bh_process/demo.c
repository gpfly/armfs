#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/interrupt.h>

#define DEVNAME "spin_bh"

#define CNT 9

struct mydev {
	struct miscdevice misc;
	int info_cnt;
	spinlock_t spin;
};

struct mydev mydev;

struct millkey{
	int irqnum;
	char *name;
	int keycnt;
}keys[] = {
	{ IRQ_EINT(26), "KEY1", 0 },
	{ IRQ_EINT(27), "KEY2", 0 },
	{ IRQ_EINT(28), "KEY3", 0 },
	{ IRQ_EINT(29), "KEY4", 0 },
};

static struct tasklet_struct task;

static void out_info(const char *arg)
{
	int i = 0; 
	
	spin_lock_bh(&mydev.spin);
	for (; i < CNT; ++i) {
		printk("%d -> %s\n", mydev.info_cnt, arg);
		mydev.info_cnt++;
		mdelay(50);
	}
	spin_unlock_bh(&mydev.spin);
}

/*irq bootm half*/
static void do_bh_handler(unsigned long data)
{
	struct millkey *pdev = (void *)data;

	out_info(pdev->name);	
}

/*irq top half*/
static irqreturn_t do_th_handler(int irqnum, void *data)
{
	task.data = (unsigned long)data;	

	tasklet_schedule(&task);

	return IRQ_HANDLED;
}

static int register_keys(void)
{
	int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(keys); ++i) {
		ret = request_irq(
			keys[i].irqnum,
			do_th_handler,
			IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
			keys[i].name,
			&keys[i]
		);

		if (ret < 0) {
			goto error0;
		}
	}

	tasklet_init(&task, do_bh_handler, 0);

	return 0;

error0:
	while (i--) {
		free_irq(keys[i].irqnum, &keys[i]);
	}

	return ret;
}

static void unregister_keys(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(keys); ++i) {
		free_irq(keys[i].irqnum, &keys[i]);
	}

	tasklet_kill(&task);
}
static int     
mill_open (struct inode *inodp, struct file *filp)
{
	return 0;
}


static long 
mill_unlocked_ioctl (struct file *filp, unsigned int request, unsigned long arg)
{
	out_info((char *)arg);

	return 0;
}

static int
mill_release (struct inode *inodp, struct file *filp)
{
	return 0;
}

static struct file_operations fops = {
	.owner 		= THIS_MODULE,
	.open		= mill_open,
	.unlocked_ioctl = mill_unlocked_ioctl,
	.release	= mill_release,
};

struct mydev mydev = {
	.info_cnt = 0,
	.misc	  = {
		.minor	= MISC_DYNAMIC_MINOR,
		.name	= DEVNAME,
		.fops	= &fops,	
	}
};

static int __init demo_init(void)
{
	int ret;

	spin_lock_init(&mydev.spin);
	
	ret = register_keys();
	if (ret < 0) {
		return ret;
	}

	if ((ret = misc_register(&mydev.misc)) < 0) {
		unregister_keys();
		return ret;
	}

	return 0;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	unregister_keys();
	misc_deregister(&mydev.misc);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
