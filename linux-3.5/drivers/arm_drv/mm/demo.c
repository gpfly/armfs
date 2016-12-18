#include <linux/init.h>
#include <linux/module.h>

#define SZ 256

static void *vir;

static int __init demo_init(void)
{
	vir = kmalloc(SZ, GFP_KERNEL);	

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
