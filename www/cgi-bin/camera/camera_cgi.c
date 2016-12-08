#include <stdio.h> 
#include <stdlib.h>
#include "cgi.h"
#include <iconv.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(void)
{
	char *sw_id;
	s_cgi *cgi;
	cgi = cgiInit();
	printf("%s%c%c\n","Content-Type:text/html;charset=utf-8",13,10); 
	printf("<TITLE>登入中...</TITLE>\n");
	printf("<h1>摄像开启中,请勿关闭...................");

	pid_t pid1;
	pid1 = fork();
	if(pid1 < 0)
		perror("fork:");
	else if (pid1 == 0)
	{
		umask(0);
		int i;
		for(i = 0;i<getdtablesize();i++)
		{
			close(i);
		}
		system("killall -9 mjpg_streamer");
		chdir("/mjpg-streamer");
		system("mjpg_streamer -i \"/mjpg/input_uvc.so\" -o \"/mjpg/output_http.so -w /www\" &");
		exit(0);
	}
	printf("<meta http-equiv=\"refresh\" content=\"1;url=../../camera.html\">");
}
