#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>

#define SZ (800*480*4)

void usage(const char *str)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "      %s device pics\n", str);
	exit(1);
}

int main(int argc, char **argv)
{
	void *pret;
	int ret;
	int fd[2];

	if (argc != 3) {
		usage(argv[0]);
	}

	/*open fb devicefile*/
	fd[0] = open(argv[1], O_RDWR);
	assert(fd[0] > 0);

	fd[1] = open(argv[2], O_RDONLY | O_NDELAY);
	assert(fd[1] > 0);

	pret = mmap(NULL, SZ, PROT_READ | PROT_WRITE, 
			MAP_SHARED, fd[0], 0);
	if (pret == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	
	int id = 0;
	while (1) {
		printf("kan %d pics!!\n", id+1);
		ret = read(fd[1], pret, SZ);
		assert(ret == SZ);
		id++;
		if (id > 11) {
			lseek(fd[1], SEEK_SET, 0);
			id = 0;
		}

		sleep(1);
	};	

        return 0;
}
