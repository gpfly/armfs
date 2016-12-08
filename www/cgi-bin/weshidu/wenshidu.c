#include <stdio.h> 
#include <stdlib.h>
#include "cgi.h"
#include <iconv.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define DATAFILE "/cgi-bin/wenshidu/wsdata.txt"
int main(void)
{
	FILE *f = fopen(DATAFILE,"r");
	int ch;
	if(f == NULL) {
		printf("%s%c%c ", "Content-Type:text/html;charset=gb2312",13,10);
		printf("< TITLE >错误 < /TITLE > ");
		printf("< P >< EM >意外错误,无法打开文件< /EM >"); }
	else {
		printf("%s%c%c ", "Content-Type:text/plain",13,10);
		while((ch=getc(f)) != EOF)
			putchar(ch);
printf("<meta http-equiv=\"refresh\" content=\"2;url=../../terms.html\">");
		fclose(f);
	}
	return 0;
}

