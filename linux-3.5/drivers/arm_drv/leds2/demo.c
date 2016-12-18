#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>

#include "ioctl.h"

#define DEVNAME "leds"

enum led{
	OFF, ON
};

static int ledgpios[] = {
	EXYNOS4X12_GPM4(0),
	EXYNOS4X12_GPM4(1),
	EXYNOS4X12_GPM4(2),
	EXYNOS4X12_GPM4(3)
};

static int     
mill_open (struct inode *inodp, struct file *filp)
{

	return 0;
}

void led_ctl(enum led cmd, int arg)
{
	if (cmd == ON) {
		gpio_set_value(ledgpios[arg-1], 0);
	} else if (cmd == OFF) {
		gpio_set_value(ledgpios[arg-1], 1);
	}
}

void led_ctl_all(enum led cmd)
{
	int i;

	for (i = 0; i < 4; i++) {
		if (cmd == ON) {
			led_ctl(ON, i+1);
		} else {
			led_ctl(OFF, i+1);
		}
	}
}

static void led_get_stat(char *buf)
{
	int i;

	for (i = 0; i < 4; i++) {
		buf[i] = gpio_get_value(ledgpios[i])?'0':'1';
	}	
}

static long 
mill_unlocked_ioctl (struct file *filp, unsigned int request, unsigned long arg)
{
	if (_IOC_TYPE(request) == LEDTYPE) {
		switch (_IOC_NR(request)) {
			case 0:
				if (arg < 1 || arg > 4) {
					return -EINVAL;
				}
				led_ctl(ON, arg);
				break;
			case 1:
				if (arg < 1 || arg > 4) {
					return -EINVAL;
				}
				led_ctl(OFF, arg);
				break;
			case 2:
				led_ctl_all(ON);
				break;
			case 3:
				led_ctl_all(OFF);
				break;
			case 4:
				led_get_stat((char *)arg);
			default:
				return -EINVAL;
		}
	}	

	return 0;
}

static int
mill_release (struct inode *inodp, struct file *filp)
{

	return 0;
}

static struct file_operations fops = {
	.owner 		= THIS_MODULE,
	.open		= mill_open,
	.unlocked_ioctl = mill_unlocked_ioctl,
	.release	= mill_release,
};

static struct miscdevice misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVNAME,
	.fops	= &fops,	
};

static int __init demo_init(void)
{
	int ret;
	int i;

	for (i = 0; i < ARRAY_SIZE(ledgpios); ++i) {
		ret = gpio_request(ledgpios[i], "led");
		if (ret < 0) {
			goto error0;
		}
		ret = gpio_direction_output(ledgpios[i], 1);
		if (ret < 0) {
			gpio_free(ledgpios[i]);
			goto error0;
		}
	}
	
	/*ioread32/ioread16/ioared8  iowrite32/iowrite16/iowrite8*/

	ret = misc_register(&misc);
	if (ret < 0) {
		goto error0;
	}	

	return 0;

error0:
	while (i--) {
		gpio_free(ledgpios[i]);
	}

	return ret;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(ledgpios); ++i) {
		gpio_set_value(ledgpios[i], 1);
		gpio_free(ledgpios[i]);
	}
	
	misc_deregister(&misc);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
