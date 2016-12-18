#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

#define LEN 16

static int num = LEN;

static int array[LEN] = {12, 23, 67};

module_param_array(array, int, &num, 0);

static int __init demo_init(void)
{
	/*申请资源，设置寄存器*/	
	int i;

	for (i = 0; i < num; ++i) {
		printk("array[%d] = %d\n", i, array[i]);
	}
	
	return 0;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	printk("goodbye, world!\n");
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
