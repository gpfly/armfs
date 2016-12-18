#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>

static int myprobe(struct platform_device *pdev)
{
	printk("myprobe...\n");

	return 0;
}

static int myremove(struct platform_device *pdev)
{
	printk("myremove...\n");

	return 0;
}

static struct platform_driver driver = {
	.probe	=	myprobe,
	.remove	=	myremove,
	.driver	=	{
		.name = "shy_bee",
	}
};

static int __init demo_init(void)
{

	return platform_driver_register(&driver);
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	platform_driver_unregister(&driver);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
