#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>

#define DEVNAME "spin"

#define CNT 9

struct mydev {
	struct miscdevice misc;
	int info_cnt;
	spinlock_t spin;
};

struct mydev mydev;

static int     
mill_open (struct inode *inodp, struct file *filp)
{
	return 0;
}

static void out_info(const char *arg)
{
	int i = 0; 
	
	spin_lock(&mydev.spin);
	for (; i < CNT; ++i) {
		printk("%d -> %s\n", mydev.info_cnt, arg);
		mydev.info_cnt++;
		mdelay(50);
	}
	spin_unlock(&mydev.spin);
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
	spin_lock_init(&mydev.spin);

	return misc_register(&mydev.misc);
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	misc_deregister(&mydev.misc);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
