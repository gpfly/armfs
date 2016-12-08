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
#include "beep.h"

#include <linux/ioctl.h>
#define BEEPTYPE	'b'
#define BEEP_SET_FREQ  	_IOW(BEEPTYPE, 0, int)
#define BEEP_STOP		_IO(BEEPTYPE, 1)

int beep_start(void)
{
	int fd;
	fd = open("/dev/changbeep", O_RDONLY);
	assert(fd > 0);
	ioctl(fd,BEEP_SET_FREQ,1000);
	close(fd);
        return 0;
}


int beep_stop(void)
{
	int fd;
	fd = open("/dev/changbeep", O_RDONLY);
	assert(fd > 0);
	ioctl(fd,BEEP_STOP);
	close(fd);
        return 0;
}
