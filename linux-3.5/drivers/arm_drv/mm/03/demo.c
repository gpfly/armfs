#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#define ORDER 11

static void *vir;

static int __init demo_init(void)
{
	vir = (void *)__get_free_pages(GFP_KERNEL, ORDER);

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

	free_pages((unsigned long)vir, ORDER);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
