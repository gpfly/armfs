#include <linux/ioctl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LEDTYPE	'L'

#define LED_SET_FREQ  	_IOW(LEDTYPE, 0, int)
#define LED_STOP		_IO(LEDTYPE, 1)
#define LED_GET_STAT	_IOR(LEDTYPE, 2, int)

char *path = "/dev/wang_led";

static int led_close(int fd)
{

	ioctl(fd, LED_STOP);
	
	return 0;
}

static int led_open(int fd, int grade)
{

	ioctl(fd, LED_SET_FREQ, grade);
	
	return 0;
}

int led_set(int grade)
{
	int fd;

	/* 0:关闭，其余都是打开，9的亮度最亮 */
	if ((grade < 0) || (grade > 9)) {
		printf("grade is 0-9!(0 is close)\n");
		return -1;
	}
		
	fd = open(path, O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	if (grade == 0) {
		led_close(fd);
	} else {
		led_open(fd, grade);
	}

	close(fd);

	return 0;
}

int led_add(int arg)
{
       arg++;
       if(arg > 9) arg = 9;
       led_set(arg);
       return 0;
}

int led_sub(int arg)
{
       arg--;
       if(arg < 0) arg = 0;
       led_set(arg);
       return 0;
}

int get_led_value(void)
{
	int ret = 0;
	int buf = 0;
	long fd = 0;

	fd = open(path, O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}
	
	ret = ioctl(fd, LED_GET_STAT, &buf);

	close(fd);

	return buf;
}


