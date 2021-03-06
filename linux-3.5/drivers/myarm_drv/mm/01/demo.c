#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define SZ 256

static void *vir;

static int __init demo_init(void)
{
	vir = kcalloc(10, sizeof(int), GFP_KERNEL);	
	if (NULL == vir) {
		return -ENOMEM;
	}

	printk("%s\n", strcpy(vir, "hehe, wanglong!\n"));

	return 0;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	printk("goodbye, world!\n");

	kfree(vir);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
