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
		chdir("/");
		system("./wenshidu_2");
		exit(0);
	}
	printf("<meta http-equiv=\"refresh\" content=\"0;url=../../main.html\">");
}
