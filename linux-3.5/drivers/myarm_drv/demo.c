#include <linux/init.h>
#include <linux/module.h>

static int __init demo_init(void)
{
	int i = CONFIG_MILLET_NUM*CONFIG_MILLET_NUM;

	while (i--) {
		printk("---------- %s --------\n", CONFIG_MILLET_INFO);
	}	

	return 0;
}

module_init(demo_init);

MODULE_LICENSE("GPL");
