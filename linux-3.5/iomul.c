#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <poll.h>

#define DEVCNT 200
#define LEN    256

static void read_fd(int fd)
{
	int ret;
	char buf[LEN+1];

	ret = read(fd, buf, LEN);
	assert(ret > 0);

	buf[ret] = '\0';
	printf("%s\n", buf);
}

int main(int argc,const char *argv[])
{
	struct pollfd pollfds[DEVCNT];
	char fdnm[64];
	int ret;

	for (int i = 0; i < DEVCNT; ++i) {
		snprintf(fdnm, 64, "/dev/scull%d", i);
		pollfds[i].fd = open(fdnm, O_RDONLY);
		assert(pollfds[i].fd > 0);
		pollfds[i].events = POLLIN;
	}

	while (1) {
		ret = poll(pollfds, DEVCNT, 5000);
		if (ret == 0) {
			printf("hehe.. timeout...\n");
		} else if (ret < 0) {
			perror("poll");
			exit(1);
		} else {
			for (int i = 0; i < DEVCNT; ++i) {
				if (pollfds[i].revents&POLLIN) {
					read_fd(pollfds[i].fd);
					ret--;
					if (ret == 0) {
						break;
					}
				}
			}
		}
	}

	return 0;
}

