#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/miscdevice.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>

#define LEDTYPE			'L'

#define LED_SET_FREQ  	_IOW(LEDTYPE, 0, int)
#define LED_STOP		_IO(LEDTYPE, 1)
#define LED_GET_STAT	_IOR(LEDTYPE, 2, int)

#define NSPERSEC		(1000000000UL)

#define DEVNAME			"wang_led"

long grade;

struct myled{
	struct pwm_device *pwm;
	struct mutex mutex;
	int id_num;
	int gpionum;
}myled = {
	.id_num = 1,
};


/* 开启 */
int led_start_work(long freq)
{
	int ret;

	grade = freq;

	int period_ns = NSPERSEC/1000;

	freq = 10 - freq;	
	
	/* 不同的频率设置不同的亮度 */
	ret = pwm_config(myled.pwm, period_ns/freq, period_ns);
	if (ret < 0) {
		return ret;
	}

	ret = s3c_gpio_cfgpin(myled.gpionum, S3C_GPIO_SFN(2));
	if (ret < 0) {
		return ret;
	}

	/* 使能 */
	ret = pwm_enable(myled.pwm);	
	if (ret < 0) {
		return ret;
	}

	return 0;	
}

/* 关闭 */
static void led_stop_work(void)
{
	grade = 0;
	pwm_disable(myled.pwm);
	gpio_direction_output(myled.gpionum, 0);
}

static int     
led_open (struct inode *inodp, struct file *filp)
{
	if (!mutex_trylock(&myled.mutex)) {
		return -EBUSY;
	}

	return 0;
}

/* GET */
static void led_get_stat(unsigned long buf)
{
	*((long *)buf) = grade;
}

static long 
led_unlocked_ioctl (struct file *filp, unsigned int request, unsigned long arg)
{
	long ret = 0;

	if (_IOC_TYPE(request) == LEDTYPE) {
		switch (_IOC_NR(request)) {
			case 0:
				ret = led_start_work(arg);
				if (ret < 0) {
					return ret;
				}
				break;
			case 1:
				led_stop_work();
				break;
			case 2:
				led_get_stat(arg);
				break;
			default:
				led_stop_work();
				return -EINVAL;
		}
	}	

	return 0;
}

static int
led_release (struct inode *inodp, struct file *filp)
{
	mutex_unlock(&myled.mutex);	

	return 0;
}

static struct file_operations fops = {
	.owner			= THIS_MODULE,
	.open			= led_open,
	.unlocked_ioctl = led_unlocked_ioctl,
	.release		= led_release,
};

static struct miscdevice misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVNAME,
	.fops	= &fops,	
};

const struct platform_device_id ledtable[] = {
	{"wangzhe",},
	{"wang_led",},
};

static int led_probe(struct platform_device *pdev)
{
	int ret = 0;

	struct resource *src = platform_get_resource_byname(pdev, IORESOURCE_MEM, "wang_led");

	int gpionum = myled.gpionum = src->start;

	if (gpio_request(gpionum, "wang_led") < 0) {
		return -EINVAL;
	}

	gpio_direction_output(gpionum, 0);

	myled.pwm = pwm_request(myled.id_num, "wang_led");
	if (IS_ERR(myled.pwm)) {
		ret = PTR_ERR(myled.pwm);
		goto error0;
	}

	if ((ret = misc_register(&misc)) < 0) {
		goto error1;
	}

	mutex_init(&myled.mutex);

	return 0;
error1:
	pwm_free(myled.pwm);
error0:
	gpio_free(gpionum);
	return ret;
}

static int led_remove (struct platform_device *pdev)
{
	led_stop_work();
	gpio_free(myled.gpionum);
	pwm_free(myled.pwm);	
	misc_deregister(&misc);

	return 0;
}

static struct platform_driver leddrv = {
	.probe	=	led_probe,
	.remove	=	led_remove,
	.driver	=	{
		.name	=  DEVNAME,
	},
	.id_table	=  ledtable, 
};

module_platform_driver(leddrv);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangZhe");
