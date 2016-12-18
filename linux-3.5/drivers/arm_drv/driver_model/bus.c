#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>

#include "mydev.h"

/*匹配成功必须返回1， 否则0*/
static int mymatch(struct device *dev, struct device_driver *drv)
{
	struct mydev *pdev = container_of(dev, struct mydev, device);

	printk("start to match %s device and %s driver!\n", 
				pdev->name, drv->name);

	return !strncmp(drv->name, pdev->name, strlen(drv->name));	
}

static struct bus_type myplatform = {
	.name	=	"myplatform",
	.match	=	mymatch,
};
EXPORT_SYMBOL_GPL(myplatform);

static int __init demo_init(void)
{

	return bus_register(&myplatform);
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	bus_unregister(&myplatform);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
