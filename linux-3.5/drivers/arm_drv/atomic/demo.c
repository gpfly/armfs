#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>

#define DEVNAME "atomic"

//static int open_cnt = 1;

static atomic_t open_cnt = ATOMIC_INIT(1);

static int     
mill_open (struct inode *inodp, struct file *filp)
{
	if (!atomic_dec_and_test(&open_cnt)) {
		atomic_inc(&open_cnt);
		return -EBUSY;
	}

	return 0;
}

static int
mill_release (struct inode *inodp, struct file *filp)
{
	atomic_inc(&open_cnt);

	return 0;
}

static struct file_operations fops = {
	.owner 		= THIS_MODULE,
	.open		= mill_open,
	.release	= mill_release,
};

static struct miscdevice misc  = {
		.minor	= MISC_DYNAMIC_MINOR,
		.name	= DEVNAME,
		.fops	= &fops,	
};

static int __init demo_init(void)
{
	return misc_register(&misc);
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	misc_deregister(&misc);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
