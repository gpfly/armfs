#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/time.h>

#define TM 5

static struct timer_list timer;
static char *str = "hehe...timeout...";

static void do_mill_timer(unsigned long data)
{
	printk("%s\n", (char *)data);

	mod_timer(&timer, jiffies+TM*HZ);
}

static int __init demo_init(void)
{
	setup_timer(&timer, do_mill_timer, (unsigned long)str);
	timer.expires = jiffies + HZ*TM;

	add_timer(&timer);	

	return 0;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	del_timer_sync(&timer);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
