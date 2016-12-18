#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>

#define DEVNAME "millkey"

struct millkey{
	int num;
	int irqnum;
	char *name;
	int keycnt;
}keys[] = {
	{ 0, IRQ_EINT(26), "KEY1", 0 },
	{ 1, IRQ_EINT(27), "KEY2", 0 },
	{ 2, IRQ_EINT(28), "KEY3", 0 },
	{ 3, IRQ_EINT(29), "KEY4", 0 },
};

static char keybuf[4] = {0};

static struct tasklet_struct task;
static int dnup_flag = 0;
static wait_queue_head_t wait;

static ssize_t 
mill_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	if (cnt != 4) {
		return -EINVAL;
	}	

	wait_event_interruptible(wait, dnup_flag != 0); /////

	if (copy_to_user(buf, keybuf, cnt)) {
		return -EINVAL;
	}

	dnup_flag = 0;

	return cnt;
}

static struct file_operations fops = {
	.owner		= THIS_MODULE,
	.read		= mill_read,
};

static struct miscdevice misc = {
	.minor	=	MISC_DYNAMIC_MINOR,
	.name	=	DEVNAME,
	.fops	=	&fops,
};

/*中断下半部准备对应按键的状态*/
static void do_bh_handler(unsigned long data)
{
	struct millkey *pdev = (void *)data;

	pdev->keycnt++;

	if ((pdev->keycnt%2) && keybuf[pdev->num] != 1) {
		keybuf[pdev->num] = 1;
		dnup_flag = 1;
		wake_up(&wait);
	} else if (!(pdev->keycnt%2) && keybuf[pdev->num] != 0){
		keybuf[pdev->num] = 0;
		dnup_flag = 1;
		wake_up(&wait);
	}
}
static irqreturn_t do_th_handler(int irqnum, void *data)
{
	task.data = (unsigned long)data;	

	tasklet_schedule(&task);

	return IRQ_HANDLED;
}

static int register_keys(void)
{
	int i;
	int ret;

	for (i = 0; i < ARRAY_SIZE(keys); ++i) {
		ret = request_irq(
				keys[i].irqnum,
				do_th_handler,
				IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING,
				keys[i].name,
				&keys[i]
				);

		if (ret < 0) {
			goto error0;
		}
	}

	tasklet_init(&task, do_bh_handler, 0);

	return 0;

error0:
	while (i--) {
		free_irq(keys[i].irqnum, &keys[i]);
	}

	return ret;
}

static void unregister_keys(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(keys); ++i) {
		free_irq(keys[i].irqnum, &keys[i]);
	}

	tasklet_kill(&task);
}

static int __init demo_init(void)
{
	int ret;

	ret = misc_register(&misc);
	if (ret < 0) {
		return ret;
	}

	ret = register_keys();

	if (ret < 0) {
		misc_deregister(&misc);
		return ret;
	}

	init_waitqueue_head(&wait);
	tasklet_init(&task, do_bh_handler, 0);

	printk("%s device ok!\n", DEVNAME);

	return 0;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	unregister_keys();
	misc_deregister(&misc);
	tasklet_kill(&task);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
