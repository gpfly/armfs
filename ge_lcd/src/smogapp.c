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
#include <smog.h>
int smog(void)
{
	int ret;
	int fd;
	int buf;
	short res;
        int i = 0;

	fd = open("/dev/changsmog", O_RDONLY);
        if (fd < 0) return -1;
	ret = read(fd, &res, sizeof(res));
		if (ret != sizeof(res)) {
			perror("read");
			exit(1);
		}
	buf = (int)((((res&0xfff)*1.0)*1000/4095)/10);
	close(fd);
	if(buf >= 100)
	buf == 99;
	i =  (buf/10) << 16;
	i |= (buf%10);
        return i;
}
