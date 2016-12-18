#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>

#define CNT 300
#define SZ  64

typedef struct psn {
	char name[SZ];
	int weigh;
	struct list_head list;
}psn_t;

LIST_HEAD(head);

static int __init demo_init(void)
{
	int i;
	psn_t *tmp;
	struct list_head *pos, *n;

	for (i = 0; i < CNT; i++) {
		tmp = kzalloc(sizeof(*tmp), GFP_KERNEL);
		if (NULL == tmp) {
			goto error0;
		}
		snprintf(tmp->name, SZ, "xiaowang%d", i+1);
		tmp->weigh = (80 + i)%50 + 80;
		list_add_tail(&tmp->list, &head);
	}

	list_for_each(pos, &head) {
		tmp = container_of(pos, psn_t, list);
		printk("name: %s\n", tmp->name);
		printk("weigh: %dkg\n", tmp->weigh);
		printk("-------------------\n");
	}

	return 0;

error0:
	list_for_each_safe(pos, n, &head) {
		tmp = container_of(pos, psn_t, list);
		kfree(tmp);
	}

	return -ENOMEM;
}

module_init(demo_init);

static void __exit demo_exit(void)
{
	struct list_head *pos, *n;
	psn_t *tmp;

	list_for_each_safe(pos, n, &head) {
		tmp = container_of(pos, psn_t, list);
		printk("%s is kfree...\n", tmp->name);
		kfree(tmp);
	}
}

module_exit(demo_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("WangLong");
MODULE_VERSION("wang plus 10");
MODULE_DESCRIPTION("It is a simple example for module.");
