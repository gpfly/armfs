#include <linux/init.h>
#include <linux/module.h>

static void get_kernel_time(void)
{
	u32 sec = jiffies/HZ;
	u32 min, hor;

	hor = sec/3600;
	min = (sec%3600)/60;
	sec = sec%60;

	printk("kernel run time: %02dh-%02dm-%02ds\n",
		hor, min, sec);
}

static int __init demo_init(void)
{
	printk("jiffies : %lu!\n", jiffies);

	get_kernel_time();

	return 0;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	printk("goodbye, world!\n");
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
