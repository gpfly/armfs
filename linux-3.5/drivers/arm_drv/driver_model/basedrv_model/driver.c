#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

#include "mydev.h"

extern struct bus_type myplatform;

static int myprobe (struct device *dev)
{
	struct mydev *pdev = container_of(dev, struct mydev, device);
	printk("match ok for %s device and %s driver!\n", 
			pdev->name, dev->driver->name);

	return 0;
}

static int myremove (struct device *dev)
{
	struct mydev *pdev = container_of(dev, struct mydev, device);
	printk("mismatch ok for %s device and %s driver!\n", 
			pdev->name, dev->driver->name);

	return 0;
}

static struct device_driver mydriver = {
	.name	=  "millet",
	.bus	=  &myplatform, 
	.probe	=  myprobe,
	.remove	= myremove,
};

static int __init demo_init(void)
{

	return driver_register(&mydriver);
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	driver_unregister(&mydriver);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
