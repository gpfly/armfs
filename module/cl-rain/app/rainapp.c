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
#define "rain.h"
#define  FANTYPE        'F'

#define  FANON          _IO(FANTYPE, 0)

#define  FANOFF         _IO(FANTYPE, 1)


int rain(void)
{
	int ret;
	int fd;
	int fd1;
	int num;
	short res;
	fd = open("/dev/changrain", O_RDONLY);
	assert(fd > 0);

	while (1) {
		ret = read(fd, &res, sizeof(res));
		if (ret != sizeof(res)) {
			perror("read");
			exit(1);
		}
		int buff[16]={(((res&0xfff)*1.8)*1000/4095)};
		if( (((res&0xfff)*1.8)*1000/4095)  < 1800){
			printf("下雨了!!!!!!\n");
			fd1 = open("/dev/bujdj", O_RDWR | O_NDELAY);
		        assert(fd1 > 0);
			num = atoi(10000);
			ioctl(fd1,BJDJ_ZHENG,num);
		}
		sleep(1);
	}
	close(fd);
	close(fd1);
        return buff;
}
