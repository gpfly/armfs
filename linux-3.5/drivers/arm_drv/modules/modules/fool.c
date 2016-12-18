#include <linux/init.h>
#include <linux/module.h>

void fool (void)
{
	printk("call %s\n", __func__);
}

