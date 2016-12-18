/*************************************************************************
	> File Name: write.c
	> Author:gpf 
	> Mail: g18720521413@163.com
	> Created Time: 2016年09月22日 星期四 15时44分04秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define SZ 256

//./a.out /dev/scull0 file
int main(int argc, char **argv)
{
	int ret;
	char buf[SZ];

        if ( argc != 3 ) {
                fprintf(stderr, "Usage %s devicefile file\n",argv[0]);
                return -1;
        }

        int fdd = open (argv[1], O_WRONLY);
        if( fdd < 0 ) {
                perror("open");
                return -1;
        }

	int fdf = open (argv[2], O_RDONLY | O_NDELAY); 
	assert(fdf > 0);	
      
	ret = read(fdf, buf, SZ);
	assert(ret > 0);

	ret = write(fdd, buf, ret);
	assert(ret > 0);

	close(fdf);
	close(fdd);
 
        return 0;
}
