#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>

#define  SZ 256

static void *vir;
static dma_addr_t phyaddress;

static int __init demo_init(void)
{
	vir = dma_alloc_coherent(NULL, SZ, &phyaddress, GFP_KERNEL);

	if (NULL == vir) {
		return -ENOMEM;
	}

	printk("%s\n", strcpy(vir, "hehe, wanglong!\n"));

	printk("vir = %p, phyaddress = %#x\n", vir, phyaddress);

	return 0;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	printk("goodbye, world!\n");

	dma_free_coherent(NULL, SZ, vir, phyaddress);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
