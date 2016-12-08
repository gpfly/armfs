#include <errno.h>
#include <sys/types.h>	
#include <sys/stat.h>	
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>    
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <string.h>
#include <malloc.h>
#include <linux/fb.h>
#include "jpeglib.h"
#include "jerror.h"
#include "ge_lcd.h"

void bitblt(void *pdst, void *psrc, unsigned long xres, unsigned long yres,int vx, int x0, int y0);
unsigned short RGB888toRGB565(unsigned char red, unsigned char green,
		unsigned char blue);
int fb_stat(int fd, int *width, int *height, int *depth);
void *fb_mmap(int fd, unsigned int screensize);
int fb_munmap(void *start, size_t length);
int fb_pixel(void *fbmem, int width, int height, int x, int y,
			 unsigned short color);

typedef struct VideoBuffer {
	unsigned char *start;
	size_t offset;
	size_t length;
} VideoBuffer;

typedef struct _tag_rgb {
	unsigned char B ; 
	unsigned char G ; 
	unsigned char R ;
	unsigned char null ; 
} tag_RGB ; 


//#define VEDIO_WIDTH   320 
//#define VEDIO_HEIGHT  240
#define VEDIO_WIDTH   640 
#define VEDIO_HEIGHT  480


#define VEDIO_DEVEICE "/dev/video16"

int main(int argc, char *argv[]) 
{ 
//        if (argc != 2) {
//                fprintf (stderr, "Usage %s device!\n", argv[0]);
//                exit(1);
//        }
	int numBufs = 0;
	int fd; //
	FILE * fd_y_file = 0;
	int a=0;
	int k = 0;
	/*============set frambuffer ==========*/
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE *infile; //jpg file fd
	unsigned char *buffer;
	int fbdev;
	char *fb_device;
	unsigned char *fbmem;
	unsigned int screensize;
	unsigned int fb_width;
	unsigned int fb_height;
	unsigned int fb_depth;
	unsigned int x;
	unsigned int y;
	char *prgb_buf = NULL ;
	
	//STEP1: OPEN VEDIO DEVICE
	//fd = open(VEDIO_DEVEICE, O_RDWR);
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		printf ("VEDIO ERROR:open error\n");
		exit (0);	
	}

	struct v4l2_format fmt;
	memset (&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_JPEG;
	fmt.fmt.pix.width  = VEDIO_WIDTH;
	fmt.fmt.pix.height = VEDIO_HEIGHT;
	if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) { 
		printf("VEDIO ERROR:set format failed\n"); 
		exit (0);	
	}

	//
	struct v4l2_requestbuffers req;
	memset(&req, 0, sizeof (req));
	req.count  = 10;
	req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory = V4L2_MEMORY_MMAP;
	if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
		perror("VEDIO ERROR:VIDIOC_REQBUFS error \n");
		exit (0);	
	}

	//
	VideoBuffer* buffers = calloc(req.count, sizeof(VideoBuffer));
	struct v4l2_buffer buf;
	for (numBufs = 0; numBufs < req.count; numBufs++) {
		memset( &buf, 0, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = numBufs;
		if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
			printf("VIDIOC_QUERYBUF error\n");
			exit (0);	
		}

		printf("buf len is %d\n", sizeof(buf));
		buffers[numBufs].length = buf.length;
		buffers[numBufs].offset = (size_t) buf.m.offset;
		buffers[numBufs].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
							MAP_SHARED, fd, buf.m.offset);
		printf(
				"buffers.length = %d,buffers.offset = %d ,buffers.start[0] = %d\n",
				buffers[numBufs].length, buffers[numBufs].offset,
				buffers[numBufs].start[0]);
		
		if (buffers[numBufs].start == MAP_FAILED) {
			perror("buffers error\n");
			exit (0);	
		}
		if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
			printf("VIDIOC_QBUF error\n");
			exit (0);	
		}

	}

	enum v4l2_buf_type type;
	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(fd, VIDIOC_STREAMON, &type) < 0) {
		printf("VIDIOC_STREAMON error\n");
		exit (0);		
	}
	
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl(fd, VIDIOC_G_FMT, &fmt) < 0) {
		printf("get format failed\n");
		return 0;
	} else {
		printf("Width = %d\n", fmt.fmt.pix.width);
		printf("Height = %d\n", fmt.fmt.pix.height);
		printf("Image size = %d\n", fmt.fmt.pix.sizeimage);
		printf("pixelformat = %d\n", fmt.fmt.pix.pixelformat);
	}


/* open fb*/
#if 1
	if ((fbdev = open("/dev/fb4", O_RDWR)) < 0) {
		return (-1);
	}
#endif
#if 1
        struct s3c_fb_user_window pos = {
                .x = 0,
                .y = 0,
        };
        struct s3c_fb_user_chroma color = {
                1,0,0,0,
        };
                
        int ret;
        ret = ioctl(fbdev, S3CFB_WIN_POSITION, &pos) ;
        if (ret < 0) {
                printf("---test1----\n");
		return (-1);
        }
        ret = ioctl(fbdev, S3CFB_WIN_SET_CHROMA, &color) ;
        if (ret < 0) {
                printf("---test2----\n");
		return (-1);
        }
#endif
        
	/*
	 * get status of framebuffer device
	 */
	fb_stat(fbdev, &fb_width, &fb_height, &fb_depth);

	/*
	 * map framebuffer device to shared memory
	 */
	screensize = fb_width * fb_height * fb_depth / 8;
	fbmem = fb_mmap(fbdev, screensize);

	//
	buffer = (unsigned char *) malloc(VEDIO_WIDTH * 6) ;
	prgb_buf = (void *)malloc (VEDIO_WIDTH * VEDIO_HEIGHT * 6) ;
	/*============set frambuffer ==========*/
	while (1) {
		for (numBufs = 0; numBufs < req.count; numBufs++) {
#if 0
			char s[15];
			sprintf(s, "%d.jpg", a);
			if ((fd_y_file = fopen(s, "wb")) < 0) {
				printf("Unable to create y frame recording file\n");
				return -1;
			}
#endif
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0) {
				perror("VIDIOC_DQBUF failed.\n");
				//return -1;
			}

			unsigned char *ptcur = buffers[numBufs].start;
#if 1	
			int i1;
			for (i1=0; i1<buf.bytesused; i1++) {
				if ((buffers[numBufs].start[i1] == 0x000000FF)
						&& (buffers[numBufs].start[i1+1] == 0x000000C4)) {
					break;
				}
			}
	
			if (i1 == buf.bytesused)
				printf("huffman table don't exist! \n");
			int i;
			for (i=0; i<buf.bytesused; i++) {
				if ((buffers[numBufs].start[i] == 0x000000FF)
						&& (buffers[numBufs].start[i+1] == 0x000000D8))
					break;
				ptcur++;
			}
#endif

			//jpeg_mem_src(&cinfo, &buffers[buf.index].start, imagesize);
			int imagesize =buf.bytesused - i;

#if 0
			fwrite(ptcur, imagesize, 1, fd_y_file);
			fclose(fd_y_file);
#endif		
			//===use frambuffer to show=============// 
#if 0
			if ((infile = fopen(s, "rb")) == NULL) {
				fprintf(stderr, "open %s failed\n", s);
				exit(-1);
			}
#endif	
			cinfo.err = jpeg_std_error(&jerr);
			jpeg_create_decompress(&cinfo);
			
			//jpeg_stdio_src(&cinfo, infile);
			jpeg_mem_src(&cinfo, buffers[numBufs].start, imagesize);
			jpeg_read_header(&cinfo, TRUE);
			jpeg_start_decompress(&cinfo);
			
			y = 0;
			while (cinfo.output_scanline < cinfo.output_height) {							 
				jpeg_read_scanlines(&cinfo, &buffer, 1);
				 //RGB 32
					memcpy((unsigned char *) prgb_buf + y * VEDIO_WIDTH * 3, buffer, cinfo.output_width * cinfo.output_components); 
				
				y++; // next scanline
			}

			//bitblt(fbmem, prgb_buf, VEDIO_WIDTH, VEDIO_HEIGHT, fb_width, 100, 100);
			bitblt(fbmem, prgb_buf, VEDIO_WIDTH, VEDIO_HEIGHT, fb_width, 0, 0);
			jpeg_finish_decompress(&cinfo);
			jpeg_destroy_decompress(&cinfo);

			if (ioctl(fd, VIDIOC_QBUF, &buf) < 0) {
				printf("VIDIOC_QBUF error\n");
			}
		}
	}
	
	free (prgb_buf);
	free (buffer);
	fb_munmap (fbmem, screensize);
	close (fbdev); // 关闭FrameBuffer设备
	close (fd);
}


void bitblt(void *pdst, void *psrc, unsigned long xres, unsigned long yres,int vx, int x0, int y0)
{
	tag_RGB *pshow = (tag_RGB *)(pdst + vx * 3 * y0 + x0 * 3) ;
	unsigned char *pssrc = (unsigned char*)(psrc ) ;
	unsigned long cha = vx - xres; 
	int x, y = 0;
	
	for (y = 0; y < yres; y ++) {
		for (x = 0; x < xres; x ++) {
		    pshow->R = *pssrc++;
		    pshow->G = *pssrc++;
		    pshow->B = *pssrc++;
		    pshow ++ ;
		}
		pshow += cha ;  
	}
}

/*
 * convert 24bit RGB888 to 16bit RGB565 color format
 */
unsigned short RGB888toRGB565(unsigned char red, unsigned char green,
		unsigned char blue) {
	unsigned short B = (blue >> 3) & 0x001F;
	unsigned short G = ((green >> 2) << 5) & 0x07E0;
	unsigned short R = ((red >> 3) << 11) & 0xF800;

	return (unsigned short) (R | G | B);
}
/*
 * get framebuffer's width,height,and depth.
 * return 0 if success, else return -1.
 */
int fb_stat(int fd, int *width, int *height, int *depth) {
	struct fb_fix_screeninfo fb_finfo;
	struct fb_var_screeninfo fb_vinfo;

	if (ioctl(fd, FBIOGET_FSCREENINFO, &fb_finfo)) {
		perror(__func__);
		return (-1);
	}

	if (ioctl(fd, FBIOGET_VSCREENINFO, &fb_vinfo)) {
		perror(__func__);
		return (-1);
	}

	*width = fb_vinfo.xres_virtual;
	*height = fb_vinfo.yres;
	*depth = fb_vinfo.bits_per_pixel;
	printf("width = %d   , height = %d  \n", *width, *height);

	return (0);
}

/*
 * map shared memory to framebuffer device.
 * return maped memory if success,
 * else return -1, as mmap dose.
 */
void * fb_mmap(int fd, unsigned int screensize) {
	caddr_t fbmem;

	if ((fbmem = mmap(0, screensize, PROT_READ | PROT_WRITE, 
	MAP_SHARED, fd, 0)) == MAP_FAILED) {
		perror(__func__);
		return (void *) (-1);
	}

	return (fbmem);
}

/*
 * unmap map memory for framebuffer device.
 */
int fb_munmap(void *start, size_t length) {
	return (munmap(start, length));
}

/*
 * display a pixel on the framebuffer device.
 * fbmem is the starting memory of framebuffer,
 * width and height are dimension of framebuffer,
 * x and y are the coordinates to display,
 * color is the pixel's color value.
 * return 0 if success, otherwise return -1.
 */
int fb_pixel(void *fbmem, int width, int height, int x, int y,
		unsigned short color) {
	if ((x > width) || (y > height))
		return (-1);

	unsigned short *dst = ((unsigned short *) fbmem + y * width + x);

	*dst = color;        
	return 0;
}
