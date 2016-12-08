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
#include <rain.h>

int  rain(void)
{
	int ret;
	int fd;
	int buff;
	short res;
	fd = open("/dev/changrain", O_RDONLY);
        if (fd < 0)       return -1;

        ret = read(fd, &res, sizeof(res));
        if (ret != sizeof(res)) {
                perror("read");
                return -1;
        }
        if( (((res&0xfff)*1.8)*1000/4095)  < 1800){
                buff = 1;		
        }else{
                buff = 0;
        }

	close(fd);
	return buff;
}
