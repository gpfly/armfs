#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <linux/proc_fs.h>
#include <linux/platform_device.h>
#include <plat/gpio-cfg.h>
#include <linux/miscdevice.h>
					
#define srd_gpin		EXYNOS4_GPX1(0)	/* GPIO 9 */
					
#define DEVNAME			"wang_fan"
					
#define FANTYPE			'F'
					
#define ON			_IO(FANTYPE, 0)
					
#define OFF			_IO(FANTYPE, 1)
					
static int fan_open (struct inode *inodp, struct file *filp)
{					
	return 0;
}					
					
static int fan_release (struct inode *inodp, struct file *filp)
{					
	return 0;		
}					
					
static int fan_on(int ctl)
{					
	gpio_set_value(srd_gpin, 0);
					
	return 0;		
}					
					
static int fan_off(int ctl)
{					
	gpio_set_value(srd_gpin, 1);
						
	return 0;			
}					
					
static long			
fan_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{					
	if (_IOC_TYPE(cmd) == FANTYPE) {
		switch(_IOC_NR(cmd)) {
			case 0:	
				fan_on(0);
				break;
			case 1:	
				fan_off(1);
				break;
			default:
				return -EINVAL;
		}			
	}				
					
	return 0;		
}					
					
static struct file_operations fops = {
		.owner			= THIS_MODULE,
		.open			= fan_open,
		.unlocked_ioctl = fan_unlocked_ioctl,
		.release		= fan_release,
};					
					
static struct miscdevice misc = 
{					
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVNAME,
	.fops	= &fops,
};					
					
static int fan_probe(struct platform_device *dev)
{					
	int ret = 0;
					
	ret = gpio_request(srd_gpin, "wangfans");
	if (ret < 0) {  
		goto error0;    
	}				
					
	gpio_direction_output(srd_gpin, 1);
		
	ret = misc_register(&misc);
	if (ret < 0) {
		goto error0;
	}

	return 0;

error0:
		gpio_free(srd_gpin);

	return -1;
}

static int fan_remove(struct platform_device *pdev)
{		   
	gpio_free(srd_gpin);
	misc_deregister(&misc);
	
	return 0;
}

static struct platform_driver wang_fan = {
	.probe	= fan_probe,
	.remove	= fan_remove,
	.driver	= {
		.name = "wang_fan",
	},
};

module_platform_driver(wang_fan);

MODULE_LICENSE("GPL");
