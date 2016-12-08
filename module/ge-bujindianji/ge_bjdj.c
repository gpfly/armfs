#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>

#include "ge_bjdj.h"

#define DEVNAME "bujdj"
unsigned int  RATE =   100;

static int bjdjgpios[] = {
        EXYNOS4_GPB(4),
        EXYNOS4_GPB(5),
        EXYNOS4_GPB(6),
        EXYNOS4_GPB(7),
        //EXYNOS4_GPX3(2),
        //EXYNOS4_GPX3(3),
        //EXYNOS4_GPX3(4),
        //EXYNOS4_GPX3(5),
};

//struct mutex mutex;

static int     
mill_open (struct inode *inodp, struct file *filp)
{
	return 0;
}

static unsigned int bjdj_get_stat (void)
{
	unsigned int i;
	for (i = 0; i < 4; i++) {
		if (gpio_get_value(bjdjgpios[i])) return ++i;
	}	
        return i;
}

void bjdj_zturn(unsigned int arg)
{
        unsigned int pos = bjdj_get_stat();
        printk("-----zheng--in------");
        printk("pos = %d\n",pos);
        while (arg--) {
		gpio_set_value(bjdjgpios[pos-1], 0);
                pos = (pos % 4) + 1;
		gpio_set_value(bjdjgpios[pos-1], 1);

                msleep_interruptible(RATE);
        }
        printk("-----zheng--out------");
}

void bjdj_fturn(unsigned int arg)
{
        unsigned int pos = bjdj_get_stat();
        printk("-----fan--in------");
        printk("pos = %d\n",pos);
        while (arg--) {
		gpio_set_value(bjdjgpios[pos-1], 0);
                if (!--pos) pos = 4;
		gpio_set_value(bjdjgpios[pos-1], 1);

                msleep_interruptible(RATE);
        }
        printk("-----fan--out------");
}

static long 
mill_unlocked_ioctl (struct file *filp, unsigned int request, unsigned long arg)
{
 //       mutex_lock (&mutex);
	if (_IOC_TYPE(request) == BJDJTYPE) {
		switch (_IOC_NR(request)) {
			case 0:
                                bjdj_zturn(arg);
				break;
			case 1:
                                bjdj_fturn(arg);
				break;
			case 2:
                                if (arg) 
                                       RATE = arg; 
				break;
			default:
                        break;
		}
	}	

  //      mutex_unlock(&mutex);
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

	for (i = 0; i < ARRAY_SIZE(bjdjgpios); ++i) {
		ret = gpio_request(bjdjgpios[i], "bujdj");
		if (ret < 0) {
			goto error0;
		}
		ret = gpio_direction_output(bjdjgpios[i], 1);
		if (ret < 0) {
			gpio_free(bjdjgpios[i]);
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
		gpio_free(bjdjgpios[i]);
	}

	return ret;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(bjdjgpios); ++i) {
		gpio_set_value(bjdjgpios[i], 1);
		gpio_free(bjdjgpios[i]);
	}
	
	misc_deregister(&misc);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("gpf_bjfj");
MODULE_VERSION("gpf_bjdj plus 0.1");
MODULE_DESCRIPTION("It is a simple bujindianji module.");
