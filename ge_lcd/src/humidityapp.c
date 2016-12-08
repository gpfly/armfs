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

#include <humidity.h>

int humidity(void)
{
	int ret;
	int fd;
	int j=0;
	int i=0;
	short res;

	fd = open("/dev/changhumidity", O_RDONLY);
        if (fd < 0 ) return -1;
	ret = read(fd, &res, sizeof(res));
	if (ret != sizeof(res)) {
		perror("read");
		exit(1);
	}
	i = (int)((((((res&0xfff)*1.0)*1000/4095))/10));
	j = (i/10) << 16;
	j |= (i%10);
	close(fd);
        return j;
}
