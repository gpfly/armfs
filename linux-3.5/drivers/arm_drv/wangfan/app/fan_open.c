#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define	 FANTYPE	'F'

#define	 ON		_IO(FANTYPE, 0)

#define	 OFF		_IO(FANTYPE, 1)

int main(int argc, const char *argv[])
{
	int ret, fd;
	
	fd = open("/dev/wang_fan", O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	printf("2222222222222222");

	ret = ioctl(fd, ON);
	if (ret < 0) {
		perror("ioctl");
		return -1;
	}

	return 0;
}
