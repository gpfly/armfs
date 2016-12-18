#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc,const char *argv[])
{
		int ret;
		char buff[128];
		
		int fd = open("/dev/scull88",O_RDONLY);
		if(fd < 0) 
			return -1;
		while(1){
			memset(buff,0,sizeof(buff));
			ret = read(fd,buff,sizeof(buff));
			printf("%s\n",buff);
			if(ret < 0)goto error;
		}

		close(fd);
		return 0;
error:
		close(fd);
		return -1;

}

