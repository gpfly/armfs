#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ge_bjdj.h"

void usage(const char *str)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "      %s zheng/fan/set rate(rate>0)\n", str);
	exit(1);
}

int main(int argc, char **argv)
{
	int fd;
	int num;
	int request;
	int ret;
	char buf[4] = {0};	
	int i;

	if (argc != 3) {
		usage(argv[0]);
	}

	fd = open("/dev/bujdj", O_RDWR | O_NDELAY);
        printf("fd:%d\n", fd);
	assert(fd > 0);

        if (!strncmp("zheng", argv[1], 5))	{
                request = BJDJ_ZHENG;
        } else if (!strncmp("fan", argv[1], 3)) {
                request = BJDJ_FAN;
        } else if (!strncmp("set", argv[1], 3)) {
                request = BJDJ_RATE;
        } else {
                usage(argv[0]);
        }

        num = atoi(argv[2]);

        ret = ioctl(fd, request, num);
        if(ret) perror("ioctl");
        close(fd);
        return 0;
}
