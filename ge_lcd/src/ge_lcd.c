#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <ge_lcd.h>
#include <ge_list.h>
#include <fb.h>

/*win 1-4 status:open(1) or close(0)*/
static int  win_status[5] = {0};

int init_win_0 (void ** p_win) 
{
        if(request_win_n(0))
                return -1;
        int fd = open ("/dev/fb0", O_RDWR);
        if (fd < 0) 
                return -1;

	*p_win = mmap(NULL, SZ, PROT_READ | PROT_WRITE, 
			MAP_SHARED, fd, 0);
	if (*p_win == MAP_FAILED) {
                goto error;
	}
        win_status[0] = 1;
        return fd;
error:
        close(fd);
        return -1;
}

int init_win_n (int fb, struct s3c_fb_user_window * pos, 
                       struct s3c_fb_user_chroma * color, 
                       void ** p_win)
{
        if(fb > 4 || fb < 1)   return -1;
        if(request_win_n(fb))  return -1;
        win_status[fb] = 1;

        int ret;
        char dev_name[8];
        int fd;
        int win_size = (800 - pos->x) * (480 - pos->y) * 4;

        sprintf (dev_name, "/dev/fb%d", fb);
        fd = open (dev_name , O_RDWR );
        if (fd < 0) {
                return fd;
        }
        
        ret = ioctl(fd, S3CFB_WIN_POSITION, pos) ;
        if (ret < 0) {
                goto error;
        }
        ret = ioctl(fd, S3CFB_WIN_SET_CHROMA, color) ;
        if (ret < 0) {
                goto error;
        }


	*p_win = mmap(NULL, win_size, PROT_READ | PROT_WRITE, 
			MAP_SHARED, fd, 0);
	if (*p_win == MAP_FAILED) {
                goto error;
	}

        memset(*p_win, 0, win_size);

        return fd;
error:
        close(fd);
        return -1;

}

int display_number (void * pwin, const char * num_buf, 
                    user_u32 num_size, user_u32 front_color, 
                    user_u32 x_offs, user_u32 y_offs,
                    struct s3c_fb_user_window * pos)
{
        if (NULL == pwin) return -1; 
        user_u32 x_num = num_size >> 16;
        user_u32 y_num = num_size & 0xffff;
        user_u32 x_scr = 800 - pos->x;
        user_u32 y_scr = 480 - pos->y;
        if ( x_offs > x_scr || y_offs > y_scr ) 
                        return -1;
        user_u32 x_min, y_min;
	user_u32 *pw;
        user_u32 back_color = 0;
        char * pn = (char *)num_buf;

        int x, y;
        x_min = (x_num + x_offs < x_scr) ? x_num : (x_scr - x_offs);
        y_min = (y_num + y_offs < y_scr) ? y_num : (y_scr - y_offs);

        pw = (user_u32 *)(pwin + (y_offs * x_scr + x_offs) * 4) ;

        for ( y = 0; y < y_min ; y++ ) {
                for ( x = 0; x < x_min; x++ ) {
                        if ( pn[y*x_num/8 + x/8] & (0x80 >> (x%8) ))
                                pw[y*x_scr + x] = front_color;
                        else pw[y*x_scr + x] = back_color;
                }
        }

        return 0;
}

int display_fb_n(void * pwin, const char * pic_buf, 
                        user_u32 x_offs, user_u32 y_offs,
                        struct s3c_fb_user_window * pos)
{
        if (NULL == pwin) return -1; 
        user_u32 x_pic = *((int *)pic_buf + 0) >> 16;
        user_u32 y_pic = *((user_u32 *)pic_buf + 0) & 0xffff;
        user_u32 x_scr = 800 - pos->x;
        user_u32 y_scr = 480 - pos->y;
        user_u32 win_size = x_scr * y_scr * 4;
        if ( x_offs > x_scr || y_offs > y_scr ) 
                        return -1;
        user_u32 x_min, y_min;
	void *pw, *pb;
        int i;

        x_min = (x_pic + x_offs < x_scr) ? x_pic : (x_scr - x_offs);
        y_min = (y_pic + y_offs < y_scr) ? y_pic : (y_scr - y_offs);
        pw = pwin + (y_offs * x_scr + x_offs) * 4 ;
        pb = (void *)pic_buf + 4;

#if 1
        if (x_scr == x_pic && x_pic == x_min) {
                memcpy (pw, pb, win_size);
        } else for (i = 0; i < y_min ; i++) {
                memcpy (pw, pb, x_min * 4);
                pw += x_scr * 4;
                pb += x_pic * 4;
        }
#endif
        return 0;
}

int close_win_n (int fb, int fd, 
                        struct s3c_fb_user_window * pos,
                        void * p_win)
{
        if(fb > 4 || fb < 1)   return -1;
        if(!request_win_n(fb))  return -1;
        int win_size = pos->x * pos->y * 4;
	munmap(p_win, win_size * 4);

        struct s3c_fb_user_chroma color = {
                0,
                0,
                0,
                0
        };
        ioctl(fd, S3CFB_WIN_SET_CHROMA, &color) ;

        close(fd);
        win_status[fb] = 0;
        return 0;
}

int add_icon_list (struct list_head * head, struct win_icon * new)
{
        mk_image_rg888 (new->file_name, &(new->pic_buf));

        list_add_tail( &new->list, head);

        return 0;
}
