#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ioctl.h"

void usage(const char *str)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "      %s on/off num(1~4)\n", str);
	fprintf(stderr, "      %s onall/offall\n", str);
	fprintf(stderr, "      %s stat\n", str);
	exit(1);
}

int main(int argc, char **argv)
{
	int fd;
	int num;
	int request;
	int ret;
	char buf[4] = {0};	
	int i;

	if ((argc != 2)&&(argc != 3)) {
		usage(argv[0]);
	}

	fd = open("/dev/leds", O_RDWR | O_NDELAY);
	assert(fd > 0);

	if (argc == 2) {
		if (!strncmp("onall", argv[1], 5)) {
			ret = ioctl(fd, LED_ON_ALL);
			assert(ret == 0);
		} else if (!strncmp("offall", argv[1], 5)) {
			ret = ioctl(fd, LED_OFF_ALL);
			assert(ret == 0);
		} else if (!strncmp("stat", argv[1], 4)){
			ret = ioctl(fd, LED_GET_STAT, buf);	
			for (i = 0; i < 4; i++) {
				if (buf[i] == '1') {
					printf("led %d is on\n", i+1);
				} else if (buf[i] == '0'){
					printf("led %d is off\n", i+1);
				} else {
					exit(1);
				}
			}
		} else {
			usage(argv[0]);
		}
	} else{
		if (!strncmp("on", argv[1], 2))	{
			request = LED_ON;
		} else if (!strncmp("off", argv[1], 3)) {
			request = LED_OFF;
		} else {
			usage(argv[0]);
		}

		num = atoi(argv[2]);

		ret = ioctl(fd, request, num);
		assert(ret == 0);
	}

        return 0;
}
