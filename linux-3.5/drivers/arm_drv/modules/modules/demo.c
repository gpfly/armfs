#include <linux/init.h>
#include <linux/module.h>

extern void fool(void);

static int __init demo_init(void)
{
	/*申请资源，设置寄存器*/	
	printk("hello, world!\n");

	fool();

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
