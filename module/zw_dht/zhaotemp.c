#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/errno.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <mach/gpio.h>
#include <asm-generic/uaccess.h>
#include <linux/spinlock_types.h>
#include <linux/mutex.h>

//#include <linux/list.h>
#define DEVICENAME "dht"

//static int wenshidu = EXYNOS4_GPB(4);
static int wenshidu = EXYNOS4_GPX3(2);

typedef unsigned char U8;
unsigned char buf[6];
unsigned char check_flag;
static DEFINE_MUTEX(mutex);


int read(void)
{
	gpio_direction_input(wenshidu);
	return gpio_get_value(wenshidu);
}


void gpio_out(int value)
{
	gpio_direction_output(wenshidu,value);
}


unsigned char one_read(void)
{
	int i=0;
	int num;
	unsigned char flag=0;
	unsigned char data=0;
	for(num=0;num<8;num++){
		i=0;
		while(!gpio_get_value(wenshidu)){
			udelay(10);
			i++;
			if(i>10)
			break;
		}
		flag=0x0;
		udelay(28);
		if(gpio_get_value(wenshidu)){
			flag=0x01;
		}
		i=0;
		while(gpio_get_value(wenshidu)){
			udelay(10);
			i++;
			if(i>12)
			break;
		}
		data<<=1;
		data|=flag;
	}
		return data;
}


void read_data(void)
{
	int i=0;
	gpio_out(0);
	mdelay(30);
	gpio_out(1);
	udelay(20);
	if(read()==0){
		while(!gpio_get_value(wenshidu)){
			udelay(5);
			i++;
			if(i>20){
				break;
			}
		}
		i=0;
		while(gpio_get_value(wenshidu)){
			udelay(5);
			i++;
			if(i>20){
				break;
			}
		}
		for(i=0;i<5;i++){
			buf[i]=one_read();
			buf[5]=buf[0]+buf[1]+buf[2]+buf[3];
		}
		if(buf[4]==buf[5]){
			check_flag=0xff;
		}
		else{
			check_flag=0x0;
		}
	}
}


static ssize_t wenshidu_read(struct file *filp, char __user *buffer, size_t size, loff_t *off)
{
	int ret;
	local_irq_disable();
	read_data();
	local_irq_enable();
	if (check_flag==0xff) {
		ret=copy_to_user(buffer,buf,sizeof(buf));
		if(ret<0){
			return -EAGAIN;	
		}else
			return 0;
	} else 
			return -EAGAIN;	
}


static int wenshidu_open(struct inode *inodp, struct file *filp)
{
	return 0;
}


static int wenshidu_release(struct inode *inodp, struct file *filp)
{
	return 0;
}

static struct file_operations fop = {
	.owner = THIS_MODULE,
	.open  = wenshidu_open,
	.read  = wenshidu_read,
	.release = wenshidu_release,
};


static struct miscdevice misc = {
	.minor = 255,
	.name = DEVICENAME,
	.fops = &fop,
};


static int wenshidu_register(void)
{
	return 0;
}

	
static int __init wenshidu_init(void)
{
	int ret;
	ret = gpio_request(wenshidu,DEVICENAME);
	if (ret) {
		return ret;
	}
	gpio_direction_output(wenshidu,1);
	ret=misc_register(&misc);
	return ret;
}


static void __exit wenshidu_exit(void)
{
	gpio_free(wenshidu);
	misc_deregister(&misc);
}


module_init(wenshidu_init);

module_exit(wenshidu_exit);

MODULE_LICENSE("GPL");
