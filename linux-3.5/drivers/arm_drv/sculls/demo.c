#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/device.h>

#define DEVCNT 		200
#define SZ     		256
#define DEVNAME 	"scull"

typedef struct scull {
	char *kbuf;
	size_t valid_len;
	struct cdev cdev;
}scull_t;

static int major = 0;
static int minor = 0;
static dev_t devnum = 0;
static struct class *class = NULL;

static scull_t *sculls_p = NULL;

static int     
mill_open (struct inode *inodp, struct file *filp)
{
	filp->private_data = container_of(inodp->i_cdev, scull_t, cdev);

	return 0;
}

static ssize_t 
mill_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	ssize_t ret;

	scull_t *pdev = filp->private_data;

	if (cnt == 0) {
		return 0;
	}
	
	if (pdev->valid_len == 0) {
		return 0;
	}

	ret = min(cnt, pdev->valid_len);	

	if (copy_to_user(buf, pdev->kbuf, ret)) {
		return -EINVAL;
	}

	memmove(pdev->kbuf, pdev->kbuf + ret, pdev->valid_len - ret);	
	pdev->valid_len -= ret;

	return ret;
}

static ssize_t
mill_write (struct file *filp, const char __user *buf, size_t cnt, loff_t *fpos)
{
	ssize_t ret;

	scull_t *pdev = filp->private_data;

	if (cnt == 0) {
		return 0;
	}

	if (pdev->valid_len == SZ) {
		return 0;
	}

	ret = min(SZ- pdev->valid_len, cnt);

	if (copy_from_user(pdev->kbuf+pdev->valid_len, buf, ret)) {
		return -EINVAL;
	}

	pdev->valid_len += ret;

	return ret;	
}

static int     
mill_release (struct inode *inodp, struct file *filp)
{

	return 0;
}

static struct file_operations fops = {
	.owner 		= THIS_MODULE,
	.open		= mill_open,
	.read		= mill_read,
	.write		= mill_write,
	.release	= mill_release,
};

static int __init demo_init(void)
{
	int ret = 0;
	int i;
	struct device *device;

	/*静态申请注册设备号*/
	if (major > 0) {
		devnum = MKDEV(major, minor);
		ret = register_chrdev_region(devnum, DEVCNT, DEVNAME);
		if  (ret < 0) {
			return ret;
		}
	} else if (major == 0){/*动态申请注册设备号*/
		ret = alloc_chrdev_region(&devnum, minor, DEVCNT, DEVNAME);
		if  (ret < 0) {
			return ret;
		}
		major = MAJOR(devnum);
	} else {
		return -EINVAL;
	}

	/*为200个设备分配结构体类型需要的空间*/
	sculls_p = kzalloc(DEVCNT*sizeof(scull_t), GFP_KERNEL);
	if (NULL == sculls_p) {
		ret = -ENOMEM;
		goto error0;
	}

	/*生成创建设备文件需要的struct class*/
	class = class_create(THIS_MODULE, DEVNAME);
	if (IS_ERR(class)) {
		ret = PTR_ERR(class);
		goto error1;
	}

	for (i = 0; i < DEVCNT; i++) {

		/*为每个设备分配256字节的空间，地址赋给kbuf*/
		sculls_p[i].kbuf = kzalloc(SZ, GFP_KERNEL);
		if (NULL == sculls_p[i].kbuf) {
			ret = -ENOMEM;
			goto error2;
		}
	
		/*初始化字符设备驱动对象及注册字符设备驱动*/
		cdev_init(&sculls_p[i].cdev, &fops);	
		ret = cdev_add(&sculls_p[i].cdev, devnum+i, 1);
		if (ret < 0) {
			kfree(sculls_p[i].kbuf);
			goto error2;
		}

		/*创建设备文件*/
		device = device_create(class, NULL, devnum+i, NULL, DEVNAME"%d", i);		
		if (IS_ERR(class)) {
			ret = PTR_ERR(device);
			kfree(sculls_p[i].kbuf);
			cdev_del(&sculls_p[i].cdev);
			goto error2;
		}
		
	}	

	return 0;
error2:
	while (i--) {
		kfree(sculls_p[i].kbuf);
		cdev_del(&sculls_p[i].cdev);
		device_destroy(class, devnum+i);
	}
	class_destroy(class);
error1:
	kfree(sculls_p);
error0:
	unregister_chrdev_region(devnum, DEVCNT);

	return ret;

}

module_init(demo_init);

static void __exit demo_exit(void)
{
	int i = DEVCNT;

	while (i--) {
		kfree(sculls_p[i].kbuf);
		cdev_del(&sculls_p[i].cdev);
		device_destroy(class, devnum+i);
	}
	class_destroy(class);
	kfree(sculls_p);
	unregister_chrdev_region(devnum, DEVCNT);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
