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

int bjdj_zheng()
{
	int fd;
	int num = 1000;
	fd = open("/dev/bujdj", O_RDWR | O_NDELAY);
        if(fd < 0) goto error;

        ioctl(fd, BJDJ_RATE, 3);
        ioctl(fd, BJDJ_ZHENG, num);
error:
        close(fd);
        return 0;
}

int bjdj_fan()
{
	int fd;
	int num = 1000;
	fd = open("/dev/bujdj", O_RDWR | O_NDELAY);
        if(fd < 0) goto error;

        ioctl(fd, BJDJ_RATE, 3);
        ioctl(fd, BJDJ_FAN, num);
error:
        close(fd);
        return 0;
}
