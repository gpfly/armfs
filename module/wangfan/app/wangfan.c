#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define	 FANTYPE	'F'

#define	 ON		_IO(FANTYPE, 0)

#define	 OFF		_IO(FANTYPE, 1)

int fan_open(void)
{
	int ret, fd;

	fd = open("/dev/wang_fan", O_RDWR);
	if (fd < 0) {
		perror("open!");
		return -1;
	}

	ret = ioctl(fd, ON);
	if (ret < 0) {
		perror("ioctl");
		return -1;
	}

	close(fd);

	return 0;
}

int fan_close(void)
{
	int ret, fd;

	fd = open("/dev/wang_fan", O_RDWR);
	if (fd < 0) {
		perror("open!");
		return -1;
	}

	ret = ioctl(fd, OFF);
	if (ret < 0) {
		perror("ioctl");
		return -1;
	}

	close(fd);

	return 0;
}


