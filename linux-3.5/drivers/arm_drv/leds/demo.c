#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/miscdevice.h>

#include "ioctl.h"

#define DEVNAME "leds"

#define GPM4BASE 0x11000000
#define GPM4CON  0x2e0
#define GPM4DAT  0x2e4

enum led{
	OFF, ON
};

static void *gpm4base = NULL;

static int     
mill_open (struct inode *inodp, struct file *filp)
{

	return 0;
}

void led_ctl(enum led cmd, int arg)
{
	u8 tmp;

	tmp = ioread8(gpm4base+GPM4DAT);
	
	if (cmd == ON) {
		tmp &= ~(1 << (arg - 1));
	} else if (cmd == OFF) {
		tmp |= (1 << (arg - 1));
	}

	iowrite8(tmp, gpm4base+GPM4DAT);
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
	u8 tmp;

	tmp = ioread8(gpm4base+GPM4DAT);

	for (i = 0; i < 4; i++) {
		buf[i] = (tmp&(1<<i))?'0':'1';
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

	unsigned int tmp;

	gpm4base = ioremap(GPM4BASE, SZ_4K);
	if (NULL == gpm4base) {
		return -EINVAL;
	}

	tmp = ioread32(gpm4base+GPM4CON);
	tmp &= ~0xffff;
	tmp |= 0x1111;
	
	iowrite32(tmp, gpm4base+GPM4CON);

	tmp = ioread8(gpm4base+GPM4DAT);
	tmp |= 0xf;
	iowrite8(tmp, gpm4base+GPM4DAT);

	
	/*ioread32/ioread16/ioared8  iowrite32/iowrite16/iowrite8*/

	ret = misc_register(&misc);
	if (ret < 0) {
		goto error0;
	}	

	return 0;

error0:
	iounmap(gpm4base);

	return ret;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	u8 tmp;

	tmp = ioread8(gpm4base+GPM4DAT);
	tmp |= 0xf;
	iowrite8(tmp, gpm4base+GPM4DAT);
	
	iounmap(gpm4base);
	misc_deregister(&misc);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
