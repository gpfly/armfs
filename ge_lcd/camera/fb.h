
#ifndef __FB_DISPLAY_H__
#define __FB_DISPLAY_H__

#define X 800 
#define Y 480
#define BPP 24



#include <linux/fb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <signal.h>
#include <setjmp.h>
#include <jpeglib.h>
#include <pthread.h>

void mkimage(unsigned char *rgb_buf,
		int x_pic, int y_pic,
		int x_scr_offs, int y_scr_offs, char ** fb_buf);

int is_bmp(char *filename);
int bmp_load(char *filename,unsigned char *rgb_buf,int x_pic,int y_pic);
int bmp_getsize(char *filename,int *x_pic,int *y_pic);

int is_jpeg(char *filename);
int jpeg_load(char *filename,unsigned char *rgb_buf,int x_pic,int y_pic);
int jpeg_getsize(char *filename,int *x_pic,int *y_pic);

extern int mk_image_rg888(char * filename, char ** buf);
#endif




