#include <stdio.h>
#include <linux/ioctl.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <strings.h>

#define		CAMERA_PATH		/pice

int wangred(void)
{
	FILE *fp = NULL;
	int fd = 0;
	int buf = 0;
	char buff[128] = {0};
	char name[128] = {0};

	fd = open("/dev/wang_red", O_RDONLY);
	if (fd < 0) {
		perror("open!");
		return -1;
	}

	fp = fopen("/qwe.txt", "r");
	if (NULL == fp) {
		perror("fopen");
		return -1;
	}

	read(fd, (void *)&buf, sizeof(buf));
	if (buf == 1) {
		sprintf(buff, "echo one > /tmp/webcom");

		system(buff);

		system("3g");	//3g 放在bin路径下
		
		usleep(1000);

		fgets(name, sizeof(name), fp);

		redlog(2, name);
	} 

	printf("buf:%d\n", buf);
	printf("photoname:%s\n", name);

	fclose(fp);
	close(fd);

	return buf;
}


