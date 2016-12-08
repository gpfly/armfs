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
#include <ge_lcd.h>

#define LEN 32
int CNT = 16;
int epfd;
struct epoll_event events[16];
int x_pos , y_pos;
int pos_flag;

int check_area (struct win_icon * icon , struct s3c_fb_user_window * pos)
{
        int x_pic = *((int *)icon->pic_buf + 0) >> 16;
        int y_pic = *((user_u32 *)icon->pic_buf + 0) & 0xffff;
        //int x_pic  = ((int) (icon->pic_buf)) >> 16;
        //int y_pic  = ((int) (icon->pic_buf)) & 0xffff;
        int x_left = pos->x + icon->x_offs;
        int y_up   = pos->y + icon->y_offs;
        printf ( "x_pic:%d,y_pic:%d,x_left:%d,y_up:%d\n",
                        x_pic, y_pic, x_left, y_up);
        if ((x_pos > x_left) && (x_pos < x_left + x_pic) 
             && (y_pos > y_up) && (y_pos < y_up + y_pic))
                return 0;
        else 
                return -1;
}


int epoll_read(int fd)
{
	int ret;
	struct input_event event;

	ret = read(fd,&event,sizeof(event));
	assert(ret > 0);

        switch(event.type){
                case EV_ABS :

                if(event.code == ABS_X || event.code == ABS_MT_POSITION_X ){
                        printf("abs:x:%d\n",event.value);
                        x_pos = event.value;
                        pos_flag |= 0x2;
                }else if(event.code == ABS_Y || event.code == ABS_MT_POSITION_Y){
                        printf("abs:y:%d\n",event.value);
                        y_pos = event.value;
                        if (pos_flag != 0)
                                pos_flag |= 0x1;
                } else if(event.code == ABS_PRESSURE){
                        printf("%s\n",event.value?"down":"up");	
                        if (event.value) pos_flag = 0;
                        else pos_flag |= 0x4;
                }
                        break;
                default:
                        break;
        }
        return 0;	
}

int putaway(int fd)
{
	int ret;
	struct input_event event;

	ret = read(fd,&event,sizeof(event));
        return 0;
}

int read_ts(void)
{
        int ret;
        int i;
        
        pos_flag = 0;

        while(1) {
                if (pos_flag == 7) break;

                ret = epoll_wait(epfd,events,CNT,-1);

                if(ret < 0){
                        return -1;
                }else if(ret == 0){
                        return -1;
                }

                for(i = 0;i < ret;i++){
                        if(events[i].events & EPOLLIN){
                                epoll_read(events[i].data.fd);	
                        }	
                        else putaway(events[i].data.fd);
                }
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


int init_ts(void)
{
	int i;
	//int ret;
	int fd;
	char file[LEN];

	epfd = epoll_create(CNT);
	assert(epfd > 0);

	for(i = 0;i < CNT;++i){
                snprintf(file,LEN,"/dev/event%d",i);
                printf("file:%s\n", file);
                fd = open(file,O_RDONLY);
                if (fd >= 0) {
                        printf ("fd : %d\n", fd);
                        epoll_add(epfd,fd);
                }
                else 
                        CNT = i;
	}
        printf ("CNT = %d\n", CNT);
        if ( CNT == 0) 
                fprintf (stderr, "no input device!\n");
        return 0;
}

