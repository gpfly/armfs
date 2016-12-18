#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/list.h>

#define PROC_NM	"millet"

static struct proc_dir_entry *entry = NULL;

struct itemhead {
	struct list_head head;
	u32 len;
}itemhead = {
	.len = 0,
};

struct item {
	struct list_head list;
	char *kbuf;
};

static int myread(char *page, char **start, off_t off,
 		  int count, int *eof, void *data)
{
	int ret = 0;
	struct list_head *pos;
	struct item *tmp;

	ret += sprintf(page+ret, "--- total Bytes is %u ---\n", itemhead.len);	
	
	list_for_each(pos, &itemhead.head) {
		tmp = container_of(pos, struct item, list);
		ret += sprintf(page+ret, "%s", tmp->kbuf);
	}

	return ret;
}

static int mywrite(struct file *file, const char __user *buffer,
			   unsigned long count, void *data)
{
	struct item *tmp;

	if (count == 0) {
		return count;
	}

	tmp = kzalloc(sizeof(*tmp), GFP_KERNEL);
	if (NULL == tmp) {
		return -ENOMEM;
	}

	tmp->kbuf = kzalloc(count+1, GFP_KERNEL);	
	if (NULL == tmp->kbuf) {
		kfree(tmp);
		return -ENOMEM;
	}

	if (copy_from_user(tmp->kbuf, buffer, count)) {
		kfree(tmp->kbuf);
		kfree(tmp);
		return -EINVAL;
	}

	tmp->kbuf[count] = '\0';

	list_add_tail(&tmp->list, &itemhead.head);

	itemhead.len += count;

	return count;
}

static int __init demo_init(void)
{

	INIT_LIST_HEAD(&itemhead.head);	

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
	struct list_head *pos, *n;
	struct item *tmp;

	remove_proc_entry (PROC_NM, NULL);

	list_for_each_safe(pos, n, &itemhead.head) {
		tmp = container_of(pos, struct item, list);
		kfree(tmp->kbuf);
		kfree(tmp);
	}
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
