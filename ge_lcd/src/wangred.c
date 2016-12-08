#include <stdio.h>
#include <linux/ioctl.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <strings.h>

#define		CAMERA_PATH		/pice

static int flag_red;

int red_stat(void)
{
        return flag_red;
}

int open_red(void)
{
        flag_red = 1;
        return 1;
}

int close_red(void)
{
        flag_red = 0;
        return 0;
}

int wangred(void)
{
	FILE *fp = NULL;
	int fd = 0;
	int buf = 0;
	char name[128] = {0};

	fd = open("/dev/wang_red", O_RDONLY);
	if (fd < 0) {
		perror("open!");
		return -1;
	}

	read(fd, &buf, sizeof(buf));

	printf("wangredbuf:%d\n", buf);

	close(fd);

	return buf;
}


