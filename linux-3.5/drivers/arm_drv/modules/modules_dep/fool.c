#include <linux/init.h>
#include <linux/module.h>

static int global = 9527;
EXPORT_SYMBOL_GPL(global);

static void fool (void)
{
	printk("call %s\n", __func__);
}

EXPORT_SYMBOL_GPL(fool);

static int __init demo2_init(void)
{
	/*申请资源，设置寄存器*/	
	printk("hello, world!\n");

	fool();

	return 0;
}

module_init(demo2_init);

static void __exit demo2_exit(void)
{
	printk("goodbye, world!\n");
}

module_exit(demo2_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
