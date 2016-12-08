#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define IRRTYPE		'I'

#define IRRON		_IOW(IRRTYPE, 0, int)

#define IRROFF		_IOW(IRRTYPE, 1, int)

int irr_open(void)
{
	int ret;
	int fd;

	fd = open("/dev/wang_irr", O_RDWR);
	if (fd < 0) {
		perror("open\n");
		return -1;
	}

	ret= ioctl(fd, IRRON);
	if (ret < 0) {
		perror("ioctl\n");
		return -1;
	}
	
	return 0;
}

int irr_close(void)
{
	int ret;
	int fd;

	fd = open("/dev/wang_irr", O_RDWR);
	if (fd < 0) {
		perror("open\n");
		return -1;
	}

	ret= ioctl(fd, IRROFF);
	if (ret < 0) {
		perror("ioctl\n");
		return -1;
	}
	
	return 0;
}


