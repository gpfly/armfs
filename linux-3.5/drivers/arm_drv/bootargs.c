#include <linux/init.h>
#include <linux/module.h>

static int do_music(char *str)
{
	int i = 50;

	if (!strcmp(str, "kusha")) {
		while (i--) {
			printk("%d> kusha...\n", i);
		}
	}

	return 0;
}

early_param("lovemusic", do_music);


