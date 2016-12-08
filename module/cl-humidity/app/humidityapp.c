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

#include"humidity.h"

int humidity(void)
{
	int ret;
	int fd;
	int fd1;
	short res;

	fd = open("/dev/changhumidity", O_RDONLY);
	assert(fd > 0);

	while (1) {
		ret = read(fd, &res, sizeof(res));
		if (ret != sizeof(res)) {
			perror("read");
			exit(1);
		}
		printf("%.2f%\n",((1000-(((res&0xfff)*1.0)*1000/4095))/10));
		int buff[8]={(int)(((1000-(((res&0xfff)*1.0)*1000/4095))/10))}; 
		if(((1000-(((res&0xfff)*1.0)*1000/4095))/10) < 30){
                   fd1=open("/dev/changbeep", O_RDONLY);
                   assert(fd1 > 0);
                   ioctl(fd1,BEEP_SET_FREQ,1000);
		}
		
		sleep(1);
	}
	close(fd);
	close(fd1);
        return buff;
}
