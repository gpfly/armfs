#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>

static void myrelease(struct device *dev)
{
	printk("release ...\n");
}

static struct platform_device device = {
	.name	= "sunshine",	
	.id	= -1,
	.dev	= {
		.release = myrelease,
	}
};

static int __init demo_init(void)
{
	return platform_device_register(&device);
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	platform_device_unregister(&device);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
