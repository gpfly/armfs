#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

struct millkey{
	int irqnum;
	char *name;
	int keycnt;
}keys[] = {
	{ IRQ_EINT(26), "KEY1", 0 },
	{ IRQ_EINT(27), "KEY2", 0 },
	{ IRQ_EINT(28), "KEY3", 0 },
	{ IRQ_EINT(29), "KEY4", 0 },
};

static struct mywork {
	struct work_struct work;
	struct millkey *currkey;
}work;

static void do_bh_handler(struct work_struct *work)
{
	struct mywork *pdev = container_of(work, struct mywork, work);

	if (in_interrupt()) {
		printk("this is interrupt ...\n");
	} else {
		printk("this is process ...\n");
	}
	
	pdev->currkey->keycnt++;
	printk("%s is %s\n", pdev->currkey->name, 
		pdev->currkey->keycnt%2?"press down":"release up");
}

static irqreturn_t do_th_handler(int irqnum, void *data)
{
	//task.data = (unsigned long)data;	

	schedule_work(&work.work);
	work.currkey = data;

	printk("%s...\n", __func__);

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

	INIT_WORK(&work.work, do_bh_handler);

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

	flush_work(&work.work);
}

static int __init demo_init(void)
{
	return register_keys();
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	unregister_keys();
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
