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

#define  SRD_GPIO      EXYNOS4_GPX1(7)	/* GPIO 10 */

#define  DEVNAME	"wang_irr"

#define IRRTYPE	'I'
  
#define IRRON	_IOW(IRRTYPE, 0, int)
#define IRROFF	_IOW(IRRTYPE, 1, int)

static int wang_open(struct inode *inodp, struct file *filp)
{
	return 0;
}

static int wang_release(struct inode *inodp, struct file *filp)
{
	return 0;
}

static int irr_ctl(int ctl)
{
	gpio_set_value(SRD_GPIO, ctl);

	return 0;
}

static long 
wang_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	if (_IOC_TYPE(cmd) == IRRTYPE) {
		switch(_IOC_NR(cmd)) {
			case 0:
				irr_ctl(0);
				break;
			case 1:
				irr_ctl(1);
				break;
			default:
				return -EINVAL;
		}
	}       

	return 0;
}

static struct file_operations fops = {
		.owner			= THIS_MODULE,
		.open			= wang_open,
		.unlocked_ioctl = wang_unlocked_ioctl,
		.release		= wang_release,
};

static struct miscdevice misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVNAME,
	.fops = &fops,
};

static int wang_probe(struct platform_device *dev)
{
	int ret = 0;

	ret = gpio_request(SRD_GPIO, "irrigates");
	if(ret < 0) {  
		 goto error0;    
	}

	gpio_direction_output(SRD_GPIO, 1);
	
	if (misc_register(&misc) < 0) {
		goto error0;
	}

	return 0;

error0:
		gpio_free(SRD_GPIO);

	return -1;
}

static int wang_remove(struct platform_device *pdev)
{
	gpio_free(SRD_GPIO);
	misc_deregister(&misc);
	    
	return 0;
}

static struct platform_driver wang_irr = {
	.probe = wang_probe,
	.remove= wang_remove,
	.driver= {
		.name = "wang_irr",
	},
};

module_platform_driver(wang_irr);

MODULE_LICENSE("GPL");

