#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

static int xres = 800;
module_param(xres, int, 0);

static int yres = 480;
module_param(yres, int, 0);

static char *str = "wang long";
module_param(str, charp, 0);

static int __init demo_init(void)
{
	/*申请资源，设置寄存器*/	
	printk("hello, %s!\n", str);

	printk("xres = %d, yres = %d\n", xres, yres);

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
