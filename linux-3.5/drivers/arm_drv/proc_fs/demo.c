#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

#define PROC_NM	"millet"

static struct proc_dir_entry *entry = NULL;

static char *kbuf = NULL;

static int myread(char *page, char **start, off_t off,
 		  int count, int *eof, void *data)
{
	int ret = 0;

	if (kbuf == NULL) {
		return 0;
	}

	ret = sprintf(page, "%s", kbuf);	

	return ret;
}

static int mywrite(struct file *file, const char __user *buffer,
			   unsigned long count, void *data)
{
	char *tmp;

	if (count == 0) {
		return count;
	}

	tmp = kzalloc(count+1, GFP_KERNEL);	
	if (NULL == tmp) {
		return -ENOMEM;
	}

	if (copy_from_user(tmp, buffer, count)) {
		return -EINVAL;
	}

	tmp[count] = '\0';

	kfree(kbuf);
	kbuf = tmp;

	return count;
}

static int __init demo_init(void)
{

	entry = create_proc_entry(
		PROC_NM,
		0644,
		NULL
	);

	entry->read_proc = myread;
	entry->write_proc = mywrite;
	
	return 0;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	remove_proc_entry (PROC_NM, NULL);
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
