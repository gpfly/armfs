#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/delay.h>


/*
 * 非睡眠延时:  mdelay, ndelay, udelay
 *
 * 睡眠延时  :  ssleep, msleep, msleep_interruptible, 
 *
 * */
static wait_queue_head_t wait;

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

/*
 *	time_before/time_before_eq
 *	time_after/time_after_eq
 */

static void jiffdelay(u32 sec)
{
	unsigned long tics = jiffies + sec*HZ;	

	while (time_before((unsigned long)jiffies, tics)) {
		schedule();	
	}
}

static void jiffsleep(u32 sec)
{
	wait_event_interruptible_timeout(wait, 0, HZ*sec);
}

static int __init demo_init(void)
{
	printk("jiffies : %lu!\n", jiffies);

	init_waitqueue_head(&wait);

	get_kernel_time();
	jiffsleep(5);
	get_kernel_time();

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
