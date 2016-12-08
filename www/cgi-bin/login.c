#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *str_len=NULL;
	int len=0;
	char buf[100]="";	
	char user[20]="";
	char passwd[20]="";
	char *u = "root";
	char *p = "uplooking";
	if( (str_len==NULL)  || (sscanf(str_len, "%d", &len)!=1) || (len>80) )
		printf("sorry!error!");
	fgets(buf, len+1, stdin);
	sscanf(buf, "Username=%[^&]&Password=%s", user,passwd);
	if( (strcmp(user,u)==0) && (strcmp(passwd,p)==0) )
	{
		printf("<Iframe src=\"http://192.168.0.151/main.html\" frameBorder=0 scrolling=no height=\"210%\" width=\"100%\" >  </iframe> ");
	}
	else
	{
		printf("<h1>Sorry! username or passwd error£¡");	
	}
		return 0;
}
