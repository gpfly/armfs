#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

#include "mydev.h"

extern struct bus_type myplatform;

static void myrelease (struct device *dev)
{
	printk("myrelease ...\n");
}

static struct mydev mydev = {
	.name	=	"millet_3",
	.device = {
		.init_name = "millet_3", 
		.bus	   = &myplatform,
		.release   = myrelease,	
	}
};

static int __init demo_init(void)
{
	return device_register(&mydev.device);
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	device_unregister(&mydev.device);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
