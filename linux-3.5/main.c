#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#define LEN 128

ssize_t ledctl(int num, int cmd)
{
	ssize_t ret;

	asm volatile (
			"mov r0,%[num]\n\t"
			"mov r1,%[cmd]\n\t"

			"svc #(0x900000+378)\n\t"

			"mov %[ret], r0\n\t"

			:[ret]"=&r"(ret)
			:[num]"r"(num),[cmd]"r"(cmd)
			:"r0","r1"
		     );

	return ret;
}


int main(int argc, char **argv)
{
	int ret;	
	int num, cmd;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s num cmd\n",
				argv[0]);
		fprintf(stderr, "      num -> [1~4], cmd ->on/off\n");
		exit(1);
	}

	num = atoi(argv[1]);

	if (!strncmp("on", argv[2], 2)) {
		cmd = 1;
	} else if (!strncmp("off", argv[2], 3)) {
		cmd = 0;
	} else {
		fprintf(stderr, "Usage: %s num cmd\n",
				argv[0]);
		fprintf(stderr, "      num -> [1~4], cmd ->on/off\n");
		exit(1);
	}	

	if (ledctl(num, cmd) < 0) {
		fprintf(stderr, "error!!!!!\n");
		exit(1);
	}

	return 0;
}
