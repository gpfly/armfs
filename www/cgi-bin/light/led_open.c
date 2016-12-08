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
	pid_t pid1;
	pid1 = fork();
	if(pid1 < 0)
		perror("fork:");
	else if (pid1 == 0)
	{
		system("killall -9 led_on");
		chdir("/cgi-bin/light");
		system("./led_on");
		exit(0);
	}
	printf("<meta http-equiv=\"refresh\" content=\"0;url=../../light.html\">");
}
