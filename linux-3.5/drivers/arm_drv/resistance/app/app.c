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

int main(int argc, char **argv)
{
	int ret;
	int fd;
	short res;

	if (argc != 2) {
		usage(argv[0]);
	}

	fd = open(argv[1], O_RDONLY);
	assert(fd > 0);

	while (1) {
		ret = read(fd, &res, sizeof(res));
		if (ret != sizeof(res)) {
			perror("read");
			exit(1);
		}

		printf("sample --> %.2fmv\n", ((res&0xfff)*1.8)*1000/4095);

		sleep(1);
	}

        return 0;
}
