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
#include"smog.h"

#define  FANTYPE        'F'

#define  FANON          _IO(FANTYPE, 0)

#define  FANOFF         _IO(FANTYPE, 1)


int smog(void)
{
	int ret;
	int fd;
	int fd1;
	short res;

	fd = open("/dev/changsmog", O_RDONLY);
	assert(fd > 0);

	while (1) {
		ret = read(fd, &res, sizeof(res));
		if (ret != sizeof(res)) {
			perror("read");
			exit(1);
		}
		int buff[16]={(((res&0xfff)*3.6)*1000/4095)};
		printf("%.2f\n",(((res&0xfff)*3.6)*1000/4095));
		if((((res&0xfff)*3.6)*1000/4095) < 400){
			printf("空气质量优！\n");
		}
		if(((((res&0xfff)*3.6)*1000/4095) > 400) && (((((res&0xfff)*3.6)*1000/4095))) < 1000){
			printf("空气质量良！\n");
		}
		if((((res&0xfff)*3.6)*1000/4095) > 1000){
			printf("空气质量差！\n");
			 fd1 = open("/dev/wang_fan", O_RDWR);
   			     if (fd1 < 0) {
               		     perror("open");
              		     return -1;
       			     }
    			    ret = ioctl(fd1, FANON);
		}
		sleep(1);
	}
	close(fd);
	close(fd1);
        return buff;
}
