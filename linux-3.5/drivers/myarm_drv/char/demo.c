#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

#define DEVNAME "xfdev"

static int major = 60;

static int     
mill_open (struct inode *inodp, struct file *filp)
{
	printk("--------- this is %d-%d device open ----------\n",
			imajor(inodp), iminor(inodp));

	return 0;
}

static ssize_t 
mill_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	printk("[kernel read]: %d\n", cnt);

	return 0;
}

static int     
mill_release (struct inode *inodp, struct file *filp)
{
	printk("--------- this is %d-%d device open ----------\n",
			imajor(inodp), iminor(inodp));

	return 0;
}

static struct file_operations fops = {
	.owner 		= THIS_MODULE,
	.open		= mill_open,
	.read		= mill_read,
	.release	= mill_release,
};

static int __init demo_init(void)
{
	return register_chrdev(major, DEVNAME, &fops);
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	unregister_chrdev(major, DEVNAME);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
