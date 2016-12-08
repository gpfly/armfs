/*************************************************************************
	> File Name: ge_lcd.h
	> Author:gpf 
	> Mail: g18720521413@163.com
	> Created Time: 2016年10月15日 星期六 21时41分06秒
 ************************************************************************/

#ifndef _GE_LCD_H
#define _GE_LCD_H
#include <sys/ioctl.h>
#include <ge_list.h>

/*fb0 size*/
#define SZ (800*480*4)

/*filename size*/
#define FILENAME_SIZE 64

#define request_win_n(nr) win_status[nr]

#define arry_size(arry) sizeof(arry)/sizeof(typeof(arry[0]))

typedef unsigned int user_u32;

/*linux-3.5/drivers/video/s3c-fb.c
* win1-4 position for ioctl request*/
#define S3CFB_WIN_POSITION		_IOW('F', 203, \
						struct s3c_fb_user_window)
/*win1-4 chroma for ioctl request*/
#define S3CFB_WIN_SET_CHROMA		_IOW('F', 205, \
						struct s3c_fb_user_chroma)


/*windows position*/
struct s3c_fb_user_window {
	int x;
	int y;
};

/*关键色*/
struct s3c_fb_user_chroma {
	int		enabled;
	unsigned char	red;
	unsigned char	green;
	unsigned char	blue;
};

struct win_icon {
        char    file_name[FILENAME_SIZE];
        char *  pic_buf;
        user_u32        x_offs;
        user_u32        y_offs;
        int (* part) (void);
        int (* part_arg) (int);
        struct list_head        list;
};

struct win_screen_image {
        char    file_name[FILENAME_SIZE];
        char *  scr_buf;
};

extern int init_win_0 (void ** p_win);

extern int init_win_n (int fb, struct s3c_fb_user_window * pos, 
                       struct s3c_fb_user_chroma * color, 
                       void ** p_win);

extern int display_number (void * pwin, const char * num_buf, 
                    user_u32 num_size, user_u32 front_color, 
                    user_u32 x_offs, user_u32 y_offs,
                    struct s3c_fb_user_window * pos);

extern int display_fb_n(void * pwin, const char * pic_buf, 
                        user_u32 x_offs, user_u32 y_offs,
                        struct s3c_fb_user_window * pos);

extern int close_win_n (int fb, int fd, 
                        struct s3c_fb_user_window * pos,
                        void * p_win);

extern int add_icon_list (struct list_head * head, struct win_icon * new);

#endif
