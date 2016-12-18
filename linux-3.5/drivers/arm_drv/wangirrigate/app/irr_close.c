#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define IRRTYPE		'I'

#define IRRON		_IOW(IRRTYPE, 0, int)

#define IRROFF		_IOW(IRRTYPE, 1, int)

int main(int argc, const char *argv[])
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

