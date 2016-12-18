#include <linux/syscalls.h>

/*asmlinkage long sys_ledctl(int num, int cmd)*/
SYSCALL_DEFINE2(ledctl, int, num, int, cmd)
{
	if ((num < 1) || (num > 4)) {
		return -EINVAL;
	}	

	if ((cmd != 0) && (cmd != 1)) {
		return -EINVAL;
	}

	printk("hehe, led %d %s\n", num, cmd?"on":"off");

	return 0;
}
