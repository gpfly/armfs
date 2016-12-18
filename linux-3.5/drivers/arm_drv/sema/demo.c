#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>

#define DEVNAME "sema"

struct mydev {
	bool is_empty;
	struct miscdevice misc;
	struct semaphore sema;
};

struct mydev mydev;

static int     
mill_open (struct inode *inodp, struct file *filp)
{
	if (down_trylock(&mydev.sema)) {
		return -EBUSY;
	}

	return 0;
}

static ssize_t 
mill_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	ssize_t ret;

	if (cnt < 0) {
		return -EINVAL;
	}

//	down(&mydev.sema);
	if (mydev.is_empty == false) {
		printk("[kernel]: reading...reading...\n");
		mdelay(50);
		mydev.is_empty = true;
		ret = cnt;
	} else {
		ret = -EINVAL;
	}
//	up(&mydev.sema);
	

	return ret;
}

static ssize_t 
mill_write (struct file *filp, const char __user *buf, size_t cnt, loff_t *fpos){
	ssize_t ret;

	if (cnt < 0) {
		return -EINVAL;
	}

//	down(&mydev.sema);
	if (mydev.is_empty) {
		printk("[kernel]: writing...writing...\n");
		mdelay(50);
		mydev.is_empty = false;
		ret = cnt;
	}else {
		ret = -EINVAL;
	}
//	up(&mydev.sema);

	return ret;
	
}

static int
mill_release (struct inode *inodp, struct file *filp)
{
	up(&mydev.sema);

	return 0;
}

static struct file_operations fops = {
	.owner 		= THIS_MODULE,
	.open		= mill_open,
	.read		= mill_read,
	.write		= mill_write,
	.release	= mill_release,
};

struct mydev mydev = {
	.is_empty = true,
	.misc	  = {
		.minor	= MISC_DYNAMIC_MINOR,
		.name	= DEVNAME,
		.fops	= &fops,	
	}
};

static int __init demo_init(void)
{
	sema_init(&mydev.sema, 3);

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
