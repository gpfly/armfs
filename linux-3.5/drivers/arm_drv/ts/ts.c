#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <plat/ft5x0x_touch.h>
#include <plat/gpio-cfg.h>

struct myft5206_ts{
	struct i2c_driver driver;
	struct input_dev *tsdev;
	struct work_struct work;
	struct i2c_client *cli;
	int irqnum;
};

extern struct myft5206_ts myts;

static void do_bh_ts(struct work_struct *work)
{
	char kbuf[32];
	int xpos, ypos;
	int ret;

	ret = i2c_master_recv(myts.cli, kbuf, sizeof(kbuf));	
	if (ret < 0) {
		return;
	}

	if (!((kbuf[3]>>6)&0x3)) {
		input_report_abs(myts.tsdev, ABS_PRESSURE, 1);
		input_report_key(myts.tsdev, BTN_TOUCH, 1);
		input_sync(myts.tsdev);
	} else if (((kbuf[3]>>6)&0x3) == 0x1) {
		input_report_abs(myts.tsdev, ABS_PRESSURE, 0);
		input_report_key(myts.tsdev, BTN_TOUCH, 0);
		input_sync(myts.tsdev);
	} else if (((kbuf[3]>>6)&0x3) == 0x2) {
		xpos = ((kbuf[3]&0xf) << 8) | kbuf[4];
		ypos = ((kbuf[5]&0xf) << 8) | kbuf[6];
		input_report_abs(myts.tsdev, ABS_X, xpos);
		input_report_abs(myts.tsdev, ABS_Y, ypos);
		input_sync(myts.tsdev);
	}

	enable_irq(myts.irqnum);
}

static irqreturn_t do_ft5206_ts(int irqnum, void *data)
{
	schedule_work(&myts.work);	
	disable_irq_nosync(irqnum);

	return IRQ_HANDLED;
}

static int ts_probe(struct i2c_client *cli, const struct i2c_device_id *id)
{
	int ret;
	int irqnum;

	struct ft5x0x_i2c_platform_data *pdata = cli->dev.platform_data;	
	unsigned int gpionum = pdata->gpio_irq;

	myts.cli = cli;

	ret = gpio_request(gpionum, "ft5206_ts");
	if (ret < 0) {
		printk("gpio request error!!\n");
	}

	ret = s3c_gpio_cfgpin(gpionum, pdata->irq_cfg);
	if (ret < 0) {
		printk("gpio cfgpin error!!\n");
		goto error0;
	}

	myts.irqnum = irqnum = gpio_to_irq(gpionum);
	if (irqnum < 0) {
		printk("get irqnum error!!\n");
		ret = irqnum;
		goto error0;
	}

	ret = request_irq(irqnum, do_ft5206_ts, IRQF_TRIGGER_FALLING, "ft5206_ts", NULL);
	if (ret < 0) {
		printk("gpio cfgpin error!!\n");
		goto error0;
	}
	
	INIT_WORK(&myts.work, do_bh_ts); 

	/*************input device********************/
	myts.tsdev = input_allocate_device();
	if (NULL == myts.tsdev) {
		printk("input allocate device error!!\n");
		ret = -ENOMEM;
		goto error1;
	}

	set_bit(EV_ABS, myts.tsdev->evbit);
	set_bit(EV_KEY, myts.tsdev->evbit);

	set_bit(ABS_X, myts.tsdev->absbit);	
	set_bit(ABS_Y, myts.tsdev->absbit);	
	set_bit(ABS_PRESSURE, myts.tsdev->absbit);	

	input_set_abs_params(myts.tsdev, ABS_X, 0, 799, 0, 0);
	input_set_abs_params(myts.tsdev, ABS_Y, 0, 479, 0, 0);
	input_set_abs_params(myts.tsdev, ABS_PRESSURE, 0, 1, 0, 0);
	
	set_bit(BTN_TOUCH, myts.tsdev->keybit);

	ret = input_register_device(myts.tsdev);
	if (ret < 0) {
		printk("input register device error!!\n");
		goto error2;
	}

	printk("myft5206_ts driver is ok!!!!!!!!!!!!!\n");

	return 0;
error2:
	input_free_device(myts.tsdev);
error1:
	free_irq(irqnum, NULL);
error0:
	gpio_free(gpionum);

	return ret;	
}

static int ts_remove(struct i2c_client *cli)
{
	return 0;
}

static const struct i2c_device_id id_table[] = {
	{"wanglong",},
	{"millet_ft5206_ts",},
};

struct myft5206_ts myts = {
	.driver = {
		.probe	=	ts_probe,
		.remove	=	ts_remove,
		.driver	= {
			.name	= 	"millet_ts"
		},
		.id_table = id_table,
	},
};

static int __init myts_init(void)
{
	return i2c_add_driver(&myts.driver);
}

module_init(myts_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
