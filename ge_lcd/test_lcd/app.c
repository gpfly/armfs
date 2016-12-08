#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>

#define CNT 100

void usage(const char *str)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "      %s device\n", str);
	exit(1);
}

int main(int argc, char **argv)
{
	int ret;
	int fd;
	struct fb_var_screeninfo var;

	if (argc != 2) {
		usage(argv[0]);
	}

	fd = open(argv[1], O_RDWR);
	assert(fd > 0);

	ret = ioctl(fd, FBIOGET_VSCREENINFO, &var);
	assert(ret == 0);

	printf("xres = %d, yres = %d\n", var.xres, var.yres);
	printf("bitperpixel = %d\n", var.bits_per_pixel);
	printf("red.offset = %d, red.length = %d\n",
			var.red.offset, var.red.length);
	printf("green.offset = %d, green.length = %d\n",
			var.green.offset, var.green.length);
	printf("blue.offset = %d, blue.length = %d\n",
			var.blue.offset, var.blue.length);
	

        return 0;
}
