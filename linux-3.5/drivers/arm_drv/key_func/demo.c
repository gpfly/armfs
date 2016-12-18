#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/input.h>
#include <linux/sched.h>
#include <linux/miscdevice.h>

struct millkey{
	int num;
	int irqnum;
	char *name;
	int keycnt;
	int code;
}keys[] = {
	{ 0, IRQ_EINT(26), "KEY1", 0 , KEY_UP},
	{ 1, IRQ_EINT(27), "KEY2", 0 , KEY_DOWN},
	{ 2, IRQ_EINT(28), "KEY3", 0 , KEY_LEFT},
	{ 3, IRQ_EINT(29), "KEY4", 0 , KEY_RIGHT},
};

/*1st*/
static struct input_dev *keydev;

static struct tasklet_struct task;

/*中断下半部准备对应按键的状态*/
static void do_bh_handler(unsigned long data)
{
	struct millkey *pdev = (void *)data;

	pdev->keycnt++;

	input_report_key(keydev, pdev->code, pdev->keycnt%2);
	input_sync(keydev);
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
	int i;

	ret = register_keys();
	if (ret < 0) {
		return ret;
	}

	tasklet_init(&task, do_bh_handler, 0);

	/*2nd*/
	keydev = input_allocate_device();
	if (NULL == keydev) {
		ret = -ENOMEM;
		goto error0;
	}

	/*3rd*/
	keydev->evbit[BIT_WORD(EV_KEY)] = BIT_MASK(EV_KEY);
	for (i = 0; i < ARRAY_SIZE(keys); ++i) {
		keydev->keybit[BIT_WORD(keys[i].code)] |= 
			BIT_MASK(keys[i].code);
	}

	/*4th*/
	ret = input_register_device(keydev);
	if (ret < 0) {
		goto error1;
	}

	return 0;
error1:
	input_free_device(keydev);
error0:
	unregister_keys();

	return ret;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	unregister_keys();
	input_unregister_device(keydev);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
