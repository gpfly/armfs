#include <stdio.h>
#include <sys/types.h>

#define  FANTYPE        'F'

#define  ON          _IO(FANTYPE, 0)

#define  OFF         _IO(FANTYPE, 1)

int zhaotemp(void) 
{

	char fdshou;
	int ret;
	int fd1;
	char buf[5];
	int i = 0;
	unsigned char wenduzhengshu = 0;
	unsigned char wenduxiaoshu = 0;
	unsigned char shiduzhengshu = 0;
	unsigned char shiduxiaoshu = 0;
	fdshou = open("/dev/dht",0);
	if(fdshou<0){
		printf("/dev/dht open fail\n");
		return 0;
	}
	while(1){
		ret=read(fdshou,buf,sizeof(buf));
		if (ret<0){
			printf("dubudao\n");	
		} 
		else{
			wenduzhengshu = buf[0];
			wenduxiaoshu = buf[1];
			shiduzhengshu = buf[2];
			shiduxiaoshu = buf[3];
			printf("温度    ：%d.%.2d\n",wenduzhengshu,wenduxiaoshu);
			printf("相对湿度：%d.%d%%\n",shiduzhengshu,shiduxiaoshu);
			buff[16]={wenduzhengshu,wenduxiaoshu,shiduzhengshu,shiduxiaoshu};
       			 }
		}
	}
	close(fdshou);
	close(fd1);
	(i << 24) = wenduzhengshu;
	(i << 16) = wenduxiaoshu;
	(i << 8) = shiduzhengshu;
	  i |=    shiduxiaoshu;
	 	
	}
	return i;
}
