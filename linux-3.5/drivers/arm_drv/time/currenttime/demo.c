#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>

/*
 *	do_gettimeofday
 *
 *	struct timespec current_kernel_time(void)
 */

static void get_kernel_time(void)
{
	u32 sec = jiffies/HZ;
	u32 min, hor;

	hor = sec/3600;
	min = (sec%3600)/60;
	sec = sec%60;

	printk("kernel run time: %02dh-%02dm-%02ds\n",
		hor, min, sec);
}

static int __init demo_init(void)
{
	struct timeval time00, time11;
	struct timespec time0, time1;
	long perd1, perd0;

	do_gettimeofday(&time00);
	mdelay(2);
	do_gettimeofday(&time11);
	perd0 = (time11.tv_usec + time11.tv_sec*1000000) - 
				(time00.tv_usec + time00.tv_sec*1000000);

	//time0 = current_kernel_time();
	//mdelay(2);
	//time1 = current_kernel_time();
	//perd1 = (time1.tv_nsec - time0.tv_nsec)/1000;
	
	printk("do_get:  delay total: %ld us\n", perd0);

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
