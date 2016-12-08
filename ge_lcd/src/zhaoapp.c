#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define  FANTYPE        'F'

#define  ON          _IO(FANTYPE, 0)

#define  OFF         _IO(FANTYPE, 1)

int zhaotemp(void) 
{

	int fdshou;
	int ret;
	unsigned char buf[5];
	int i = 0;
	int wenduzhengshu = 0;
	int wenduxiaoshu = 0;
	int shiduzhengshu = 0;
	int shiduxiaoshu = 0;
	fdshou = open("/dev/dht",0);
	if(fdshou<0){
		printf("/dev/dht open fail\n");
		return -1;
	}

        ret=read(fdshou,buf,sizeof(buf));
        if (ret<0){
                printf("dubudao\n");	
        } 
        else{
                wenduzhengshu = buf[0];
                wenduxiaoshu = buf[1];
                shiduzhengshu = buf[2];
                shiduxiaoshu = buf[3];
        }

      
	i |= (wenduzhengshu/10) << 24;
	i |= (wenduzhengshu%10) << 16;
	i |= (shiduzhengshu/10) << 8;
	i |= (shiduzhengshu%10);
  	 close(fdshou);
	return i;
}
