#include <linux/module.h>
#include <linux/platform_device.h>
#include <mach/irqs.h>

#define WDT_BASE	0x10060000
#define WDT_SIZE	0x400

static void wdt_release(struct device *dev)
{
	printk("Release abc4412 wdt's pdev\n");
}

//看门狗资源数组
static struct resource wdt_res[] = {
	[0] = {
		.start = WDT_BASE,
		.end = WDT_BASE + WDT_SIZE - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_WDT,
		.end = IRQ_WDT,
		.flags = IORESOURCE_IRQ,
	},
};

//准备platform_device
static struct platform_device wdt_pdev = {
	.name = "abc4412-wdt",
	.id = -1,
	.num_resources = ARRAY_SIZE(wdt_res),
	.resource = wdt_res,
	.dev = {
		.release = wdt_release,
	},
};

module_driver(wdt_pdev, platform_device_register, platform_device_unregister);

MODULE_AUTHOR("millet");
MODULE_LICENSE("GPL");
