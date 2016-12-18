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
	int fd;
	char buf[LEN];
	pid_t pids[CNT];

	if (argc != 2) {
		usage(argv[0]);
	}

	fd = open(argv[1], O_RDONLY | O_NDELAY);
	assert(fd > 0);

	for (i = 0; i < CNT; i++) {
		if ((pids[i] = fork()) < 0) {
			perror("fork");
			exit(1);
		} 

		if (pids[i] == 0) {

			snprintf(buf, LEN, "hehe process id is %d\n", getpid());
			ret = ioctl(fd, 0, buf);	
	
			if (ret < 0) {
				perror("[app] ioctl");
				exit(1);
			}
			exit(0);
		}	
	}

	for (i = 0; i < CNT; i++) {
		waitpid(pids[i], NULL, 0);
	}

        return 0;
}
