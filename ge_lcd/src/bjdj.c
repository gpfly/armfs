#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <ge_bjdj.h>
int flag_bjdj;
int bjdj_stat(void)
{
        int stat = !!flag_bjdj;
        return stat;
}

int bjdj_zheng(void)
{
	int fd;
	int num = 1000;
	fd = open("/dev/bujdj", O_RDWR | O_NDELAY);
        if(fd < 0) goto error;
        printf("-0-bjdjzheng fd:%d\n", fd);

        ioctl(fd, BJDJ_RATE, 3);

        ioctl(fd, BJDJ_ZHENG, num);
        flag_bjdj = 1;
error:
        close(fd);
        return 0;
}

int bjdj_fan(void)
{
	int fd;
	int num = 1000;
	fd = open("/dev/bujdj", O_RDWR | O_NDELAY);
        if(fd < 0) goto error;
        printf("-0-bjdjfan fd:%d\n", fd);

        ioctl(fd, BJDJ_RATE, 3);
        ioctl(fd, BJDJ_FAN, num);
        flag_bjdj = 0;
error:
        close(fd);
        return 0;
}
