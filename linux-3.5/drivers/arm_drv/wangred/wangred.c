#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/io.h>
#include <linux/gpio.h>

#define	gpio_int   EXYNOS4_GPA1(4)		/* CON4 */

static ssize_t red_read(struct file *filp, char __user *buf, size_t cnt, loff_t *fops)
{
	int ret = 0;

	ret = gpio_get_value(gpio_int);
	if (copy_to_user(buf, &ret, sizeof(ret))) {
		return -EINVAL;
	}

	return 0;
}

static struct file_operations fops = {
	.owner  = THIS_MODULE,
	.read   = red_read,
};

static struct miscdevice misc = {
	.minor  = MISC_DYNAMIC_MINOR,
	.name   = "wang_red",
	.fops   = &fops,
};

static int __init red_init(void)
{
	int ret = 0;

	ret = gpio_request(gpio_int, "wangred");
	if (ret < 0) {
		goto error0;
	}

	ret = gpio_direction_input(gpio_int);
	if (ret > 0) {
		goto error0;
	}	
	
	ret = misc_register(&misc);
	if (ret < 0) {
		goto error0;
	}

	return 0;
error0:
	gpio_free(gpio_int);

	return -EINVAL;
}
module_init(red_init);

static void __exit red_exit(void)
{
	gpio_free(gpio_int);
	misc_deregister(&misc);
}
module_exit(red_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wangzhe");
