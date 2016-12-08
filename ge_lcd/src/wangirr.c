#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define IRRTYPE		'I'

#define IRRON		_IOW(IRRTYPE, 0, int)

#define IRROFF		_IOW(IRRTYPE, 1, int)

int flag_irr;
int irr_stat(void)
{
        return flag_irr;
}

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
        flag_irr = 1;
        close(fd);
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
        flag_irr = 0;
        close(fd);
	return 0;
}


