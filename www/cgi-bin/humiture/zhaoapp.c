#include <stdio.h>
#include <sys/types.h>

void main()
{

	char fdshou;
	int ret;
	char buf[5];
	unsigned char wenduzhengshu = 0;
	unsigned char wenduxiaoshu = 0;
	unsigned char shiduzhengshu = 0;
	unsigned char shiduxiaoshu = 0;
	fdshou = open("/dev/dht",0);
/*	if(fdshou<0){
		printf("/dev/dht open fail\n");
		return 0;
	}
*/	while(1){
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
		}
		sleep(2);
	}
	close(fdshou);
//	return 0;
}
