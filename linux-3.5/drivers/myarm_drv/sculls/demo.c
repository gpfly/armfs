#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/device.h>

/*次设备个数*/
#define DEVCNT 		200

/*每个设备缓存的大小*/
#define SZ     		256

/*设备名*/
#define DEVNAME 	"scull"

/*设备对像类型
* kbuf 内核缓存
* valid_len  有效长度
* cdev  设备类型结构*/
typedef struct scull {
	char *kbuf;
	size_t valid_len;
	struct cdev cdev;
}scull_t;

/*主设备号*/
static int major = 0;

/*次设备号*/
static int minor = 0;

/*设备号*/
static dev_t devnum = 0;

static struct class *class = NULL;

/*指向设备类型的指针*/
static scull_t *sculls_p = NULL;

/*驱动的打开设备函数，两个参数是内核传来
* inodep        是与硬件有关
* filp          是内核操作文件的类结构体
* 把硬件和内核的file结构体连结起来，使能一套驱动操作多个设备*/
static int     
mill_open (struct inode *inodp, struct file *filp)
{
        /*filp->private_data    每打开一个文件，内核会为此次打开创建一个file结构体
         *                      filp->private_data存放设备内存起始地址,使一套驱动
         *                      可以操作多个类似设备
        * container_of()        求出该设备的内存起始地址*/
	filp->private_data = container_of(inodp->i_cdev, scull_t, cdev);

	return 0;
}

/*驱动的读取函数
* struct file_operations 
* */
static ssize_t 
mill_read (struct file *filp, char __user *buf, size_t cnt, loff_t *fpos)
{
	ssize_t ret;

        /*拿到此次设备的起始地址*/
	scull_t *pdev = filp->private_data;

        /*读取字节数*/
	if (cnt == 0) {
		return 0;
	}
	
        /*此次设备内存数据的有效长度*/
	if (pdev->valid_len == 0) {
		return 0;
	}

        /*从读取字节数和有效字节数取出较小的那个*/
	ret = min(cnt, pdev->valid_len);	

        /*拷贝ret个字节数据到用户空间的buf*/
	if (copy_to_user(buf, pdev->kbuf, ret)) {
		return -EINVAL;
	}

        /*剩余有效字节前移
        * memmove　有效防止数据被覆盖*/
	memmove(pdev->kbuf, pdev->kbuf + ret, pdev->valid_len - ret);	
        /*更新有效数据*/
	pdev->valid_len -= ret;

	return ret;
}

/*驱动的写入耿耿数*/
static ssize_t
mill_write (struct file *filp, const char __user *buf, size_t cnt, loff_t *fpos)
{
	ssize_t ret;

        /*拿到此次设备的起始地址*/
	scull_t *pdev = filp->private_data;

        /*写入的字节数*/
	if (cnt == 0) {
		return 0;
	}

        /*空间满了*/
	if (pdev->valid_len == SZ) {
		return 0;
	}

        /*可写字节数与要写字节数取小*/
	ret = min(SZ- pdev->valid_len, cnt);

        /*从用户空间拷贝ret字节数据到内核*/
	if (copy_from_user(pdev->kbuf+pdev->valid_len, buf, ret)) {
		return -EINVAL;
	}

        /*更新有效数据*/
	pdev->valid_len += ret;

	return ret;	
}

/*驱动的设备关闭函数，类似于close() */
static int     
mill_release (struct inode *inodp, struct file *filp)
{

	return 0;
}

/*驱动函数集 */
static struct file_operations fops = {
	.owner 		= THIS_MODULE,
        /*此驱动的打开设备函数*/
	.open		= mill_open,
        /*此驱动的读取设备函数*/
	.read		= mill_read,
        /*此驱动的写入设备函数*/
	.write		= mill_write,
        /*此驱动的关闭设备函数*/
	.release	= mill_release,
};

/*驱动的初始化函数*/
static int __init demo_init(void)
{
	int ret = 0;
	int i;
	struct device *device;

        /*如果主设备号大于0，静态申请
        * 否则，动态申请*/
	if (major > 0) {
		devnum = MKDEV(major, minor);   //获得设备号，高12位存主设备号，低20位存次设备号
		ret = register_chrdev_region(devnum, DEVCNT, DEVNAME);  //静态申请注册设备号
		if  (ret < 0) {
			return ret;
		}
	} else if (major == 0){
                /*如果主设备号等于0，静态申请注册设备号*/
		ret = alloc_chrdev_region(&devnum, minor, DEVCNT, DEVNAME);
		if  (ret < 0) {
			return ret;
		}
		major = MAJOR(devnum);  //动态申请成功后，取出其主设备号
                                        //MINOR（devnum）,取出其次设备号
	} else {
		return -EINVAL;         //主设备号没有负的
	}

        /*申请所有次设备的空间，kzalloc申请的会把空间清零,GFP_KERNEL申
        * 请不到空间会阻塞*/
	sculls_p = kzalloc(DEVCNT*sizeof(scull_t), GFP_KERNEL);
	if (NULL == sculls_p) {
		ret = -ENOMEM;
		goto error0;
	}

        /*自动生成设备文件需要的函数
        * 1、class_create 类
        * 2、device_create 设备文件创建*/
	class = class_create(THIS_MODULE, DEVNAME);
	if (IS_ERR(class)) {
		ret = PTR_ERR(class);
		goto error1;
	}

        /*给每一个次设备申请256字节空间，*/
	for (i = 0; i < DEVCNT; i++) {
		sculls_p[i].kbuf = kzalloc(SZ, GFP_KERNEL);
		if (NULL == sculls_p[i].kbuf) {
			ret = -ENOMEM;
			goto error2;
                }

                /*给每个次设备驱动初始化并添加驱动函数集*/
                cdev_init(&sculls_p[i].cdev, &fops);	
                /*添加一个字符设备到系统，
                * devnum+i  给次设备号+1
                * 1     一次添加一个
                * 体现在/proc/devices*/
                ret = cdev_add(&sculls_p[i].cdev, devnum+i, 1);
                if (ret < 0) {
                        kfree(sculls_p[i].kbuf);
                        goto error2;
                }

                /*创建一个设备并在文件系统中申请它
                * This function can be used by char device classes.  A struct device
                * will be created in sysfs, registered to the specified class.
                * 体现在/dev/scull* */
		device = device_create(class, NULL, devnum+i, NULL, DEVNAME"%d", i);		
		if (IS_ERR(class)) {
			ret = PTR_ERR(device);
                        /*释放次设备空间*/
			kfree(sculls_p[i].kbuf);
                        /*删除一个设备*/
			cdev_del(&sculls_p[i].cdev);
			goto error2;
		}
		
	}	

	return 0;
error2:
	while (i--) {
		kfree(sculls_p[i].kbuf);
		cdev_del(&sculls_p[i].cdev);
                /*在文件系统注销某个类对应设备号的设备*/
		device_destroy(class, devnum+i);
	}
	class_destroy(class);
error1:
	kfree(sculls_p);
error0:
        /*销毁设备号*/
	unregister_chrdev_region(devnum, DEVCNT);

	return ret;

}

/*宏，指定驱动函数的入口，插入驱动后会执行demo_init*/
module_init(demo_init);

/*驱动的退出函数，在移除驱动时会执行此函数*/
static void __exit demo_exit(void)
{
	int i = DEVCNT;

	while (i--) {
                /*释放次设备空间*/
		kfree(sculls_p[i].kbuf);
                /*删除设备*/
		cdev_del(&sculls_p[i].cdev);
                /*删除设备文件/dev */
		device_destroy(class, devnum+i);
	}
        /*设备类销毁*/
	class_destroy(class);
        /*释放总设备空间*/
	kfree(sculls_p);
        /*注销设备号*/
	unregister_chrdev_region(devnum, DEVCNT);
}

/*宏，指定驱动函数的出口，移除驱动后会执行demo_exit*/
module_exit(demo_exit);

/*此驱动的许可证，必需有*/
MODULE_LICENSE("GPL");

/*此驱动的作者*/
MODULE_AUTHOR("WangLong");
/*此驱动的版本号*/
MODULE_VERSION("wang plus 10");
/*此驱动的说明*/
MODULE_DESCRIPTION("It is a simple example for module.");
