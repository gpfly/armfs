#include <linux/input.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>
#include <assert.h>

#define CNT 3
#define LEN 32

int epoll_read(int fd)
{
	int ret;
	struct input_event event;

	ret = read(fd,&event,sizeof(event));
	assert(ret > 0);

	switch(event.type){
	case EV_SYN :
		printf("----------syn-------------\n");
		break;
	case EV_ABS :
		if(event.code == ABS_X || event.code == ABS_MT_POSITION_X ){
			printf("x:%d\n",event.value);
		}else if(event.code == ABS_Y || event.code == ABS_MT_POSITION_Y){
			printf("y:%d\n",event.value);
		}else if(event.code == ABS_PRESSURE){
			printf("%s\n",event.value?"down":"up");	
		}
		break;
	case EV_REL :
		if(event.code == REL_X){
			printf("x:%d\n",event.value);
		}else if(event.code == REL_Y){
			printf("y:%d\n",event.value);
		}	
		break;
	case EV_KEY :
		printf("code%d key is %s\n",event.code,event.value?"down":"up");
		break;
	default:
		break;
	}
	return 0;	
}

int epoll_add(int epfd,int fd)
{
	int ret;
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = fd;
	ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
	assert(ret == 0);
	return 0;
}

int main(void)
{
	int i;
	int ret;
	int fd;
	char file[LEN];
	struct epoll_event events[CNT];

	int epfd = epoll_create(CNT);
	assert(epfd > 0);

	for(i = 0;i < CNT;++i){
		snprintf(file,LEN,"/dev/input/event%d",i);
			fd = open(file,O_RDONLY);
			assert(fd > 0);
			epoll_add(epfd,fd);
	}

	while(1){
		ret = epoll_wait(epfd,events,CNT,10000);
		if(ret < 0){
			exit(1);	
		}else if(ret == 0){
			printf("timeout\n");
			continue;
		}
		for(i = 0;i < ret;i++){
			if(events[i].events & EPOLLIN){
				epoll_read(events[i].data.fd);	
			}	
		}

	}
	
}

