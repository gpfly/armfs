#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/miscdevice.h>

#include "ioctl.h"

#define NSPERSEC 1000000000UL

#define DEVNAME "beep"

struct mybeep{
	struct pwm_device *pwm;
	struct mutex mutex;
	int id_num;
	int gpionum;
}mybeep = {
	.id_num = 0,
};

int beep_start_work(long freq)
{
	int ret;

	int period_ns = NSPERSEC/freq;

	ret = pwm_config(mybeep.pwm, period_ns>>1, period_ns);

	if (ret < 0) {
		return ret;
	}

	ret = s3c_gpio_cfgpin(mybeep.gpionum, S3C_GPIO_SFN(2));
	if (ret < 0) {
		return ret;
	}

	ret = pwm_enable(mybeep.pwm);	
	if (ret < 0) {
		return ret;
	}

	return 0;	
}

static void beep_stop_work(void)
{
	pwm_disable(mybeep.pwm);
	gpio_direction_output(mybeep.gpionum, 0);
}

static int     
mill_open (struct inode *inodp, struct file *filp)
{
	if (!mutex_trylock(&mybeep.mutex)) {
		return -EBUSY;
	}

	return 0;
}

static long 
mill_unlocked_ioctl (struct file *filp, unsigned int request, unsigned long arg)
{
	long ret = 0;

	if (_IOC_TYPE(request) == BEEPTYPE) {
		switch (_IOC_NR(request)) {
			case 0:
				ret = beep_start_work(arg);
				if (ret < 0) {
					return ret;
				}
				break;
			case 1:
				beep_stop_work();
				break;
			default:
				beep_stop_work();
				return -EINVAL;
		}
	}	

	return 0;
}

static int
mill_release (struct inode *inodp, struct file *filp)
{
	mutex_unlock(&mybeep.mutex);	

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

const struct platform_device_id beeptable[] = {
	{"wanglong",},
	{"millbeep",},
};

static int mill_probe(struct platform_device *pdev)
{
	int ret = 0;

	struct resource *src = platform_get_resource_byname(pdev, IORESOURCE_MEM, "millbeep");

	int gpionum = mybeep.gpionum = src->start;	

	if (gpio_request(gpionum, "millbeep") < 0) {
		printk("gpio request error!!\n");
		return -EINVAL;
	}

	gpio_direction_output(gpionum, 0);

	mybeep.pwm = pwm_request(mybeep.id_num, "millbeep");
	if (IS_ERR(mybeep.pwm)) {
		printk("pwm request error!!\n");
		ret = PTR_ERR(mybeep.pwm);
		goto error0;
	}

	if ((ret = misc_register(&misc)) < 0) {
		printk("misc register error!!\n");
		goto error1;
	}

	mutex_init(&mybeep.mutex);

	printk("%s driver ok!\n",DEVNAME);

	return 0;
error1:
	pwm_free(mybeep.pwm);
error0:
	gpio_free(gpionum);
	return ret;
}

static int mill_remove (struct platform_device *pdev)
{
	beep_stop_work();
	gpio_free(mybeep.gpionum);
	pwm_free(mybeep.pwm);	
	misc_deregister(&misc);

	return 0;
}

static struct platform_driver beepdrv = {
	.probe	=	mill_probe,
	.remove	=	mill_remove,
	.driver	=	{
		.name	=  DEVNAME,
	},
	.id_table	=  beeptable, 
};

module_platform_driver(beepdrv);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
