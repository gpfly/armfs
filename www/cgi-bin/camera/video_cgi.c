#include <stdio.h>
#include <stdlib.h>
#include <iconv.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cgi.h>
int main(void)
{	
	char *sw_id;
	s_cgi *cgi;
	pid_t pid;
//	cgi = cgiInit ();
	printf("%s%c%c\n","Content-Type:text/html;charset=utf-8",13,10);
	printf("<TITLE> video ...</TITLE>\n");
	printf("<h1>摄像开启中,请勿关闭...................");	
	printf("<img src=\"/www/images/fairyland.jpg\" width=\"640\" height=\"480\" />");
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
		chdir("/source/rootfs/bin");
		system("./mjpg_streamer -i \"./input_uvc.so\" -o \"./output_http.so -w /www\" &");
//		printf("<meta http-equiv=\"refresh\" content=\"0;url=../main.html\">");
		exit(0);
	}
	printf("<meta http-equiv=\"refresh\" content=\"1;url=../../main.html\">");

}
