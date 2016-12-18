#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <plat/adc.h>

#define DEVNAME "changhumidity"

struct resistance {
	struct s3c_adc_client *client;
	int channal;
	struct mutex mutex;
}myres = {
	.channal = 2,
};


static int     
mill_open (struct inode *inodp, struct file *filp)
{
	if (!mutex_trylock(&myres.mutex)) {
		return -EBUSY;
	}

	return 0;
}

static ssize_t 
mill_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	u16 result;

	if (cnt != 2) {
		return -EINVAL;
	}

	result = s3c_adc_read(myres.client, myres.channal);
	if (result < 0) {
		return -EINVAL;
	}

	if (put_user(result, (u16 *)buf)) {
		return -EINVAL;
	}

	return cnt;
	
}

static int mill_release (struct inode *inodp, struct file *filp)
{
	mutex_unlock(&myres.mutex);

	return 0;
}

static struct file_operations fops = {
	.owner 		= THIS_MODULE,
	.open		= mill_open,
	.read		= mill_read,
	.release	= mill_release,
};

static struct miscdevice misc = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVNAME,
	.fops	= &fops,	
};

static int res_probe (struct platform_device *pdev)
{
	int ret;

	myres.client = s3c_adc_register(pdev, NULL, NULL, 0);
	if (IS_ERR(myres.client)) {
		return -EINVAL;
	}

	mutex_init(&myres.mutex);

	ret = misc_register(&misc);
	if (ret < 0) {
		goto error0;
	}

	return 0;

error0:
	s3c_adc_release(myres.client);	

	return ret;
}

static int res_remove (struct platform_device *pdev)
{
	s3c_adc_release(myres.client);	
	misc_deregister(&misc);

	return 0;

}

static struct platform_driver resdrv = {
	.probe	=	res_probe,
	.remove	=	res_remove,
	.driver = {
		.name	= "changhumidity",
	},
};

module_platform_driver(resdrv);


MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
