#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h> 
#include <plat/irqs.h> 
#include <linux/platform_device.h>

static void myrelease(struct device *dev)
{
	printk("release ...\n");
}

static struct resource myresourc[] = {
	{
		.start	= 0x0ce00000,
		.end	= 0x0ce00043,
		.name	= "nandflash",
		.flags	= IORESOURCE_MEM,		
	},
	{
		.start	= IRQ_EINT(26),
		.end	= IRQ_EINT(29),
		.name	= "key",
		.flags	= IORESOURCE_IRQ,		
	},
	{
		.start	= 0x10060000,
		.end	= 0x1006000f,
		.name	= "watchdog",
		.flags	= IORESOURCE_MEM,		
	},
};

static struct platform_device device = {
	.name		= "shy_bee",	
	.id		= -1,
	.resource 	= myresourc,
	.num_resources  = ARRAY_SIZE(myresourc),
	.dev		= {
		.release= myrelease,
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
