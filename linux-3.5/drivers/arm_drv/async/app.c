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

#define CNT 100

static int fd;
static char oldbuf[4] = {0};

void do_sigio_handler(int signum)
{
	int i,ret;
	char currbuf[4];

	ret = read(fd, currbuf, 4);
	assert(ret == 4);

	for (i = 0; i < 4; i++) {
		if (currbuf[i] != oldbuf[i]) {
			printf("key %d is %s\n", i+1,
				currbuf[i]?"down":"up");
			oldbuf[i] = currbuf[i];
		}
	}
}

void usage(const char *str)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "      %s device\n", str);
	exit(1);
}

#define LEN 100

int main(int argc, char **argv)
{
	int i;
	int ret;
	char buf[LEN];
	pid_t pids[CNT];

	if (argc != 2) {
		usage(argv[0]);
	}

	fd = open(argv[1], O_RDONLY | O_NDELAY);
	assert(fd > 0);

        signal(SIGIO, do_sigio_handler);

	fcntl(fd, F_SETOWN, getpid());
	
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_ASYNC);

	while (1) {
		sleep(200);
	}
	
	return 0;
}
