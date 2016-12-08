#include <fb.h>

#define ERROR(condition,msg)	\
	if(condition)			\
{				\
	fprintf(stderr,"%s: %d: %s: %s\n",__FILE__,__LINE__,msg,strerror(errno));	\
	exit(-1);		\
}


void mkimage(unsigned char *rgb_buf,
		int x_pic, int y_pic, 
		int x_scr_offs, int y_scr_offs, char ** fb_buf)
{
	unsigned short *image16_buf = NULL;
	unsigned int *image32_buf = NULL;
	unsigned char *image_buf = NULL;
	unsigned char *image_buf_cur = NULL;

	unsigned char *fb_buf_cur = NULL;

	int i;	

	int x_scr;
	int y_scr;
	int screen_pixel;
	int pic_pixel;
	int x_min;
	int y_min;
	int cpp;


	if(BPP == 24)
	{
		cpp = 4;
	}
	else if(BPP == 16)
	{
		cpp = 2;
	}
	else
	{
		printf("BPP is not 16 or 24...\n");
		exit(-1);
	}


	x_scr = x_pic;
	y_scr = y_pic;
	screen_pixel = x_pic * y_pic;
	pic_pixel = x_pic * y_pic;


	image_buf = calloc(pic_pixel,cpp);

	switch(BPP)
	{
		case 16:
			image16_buf = (void *)image_buf;
			for(i = 0;i < pic_pixel;i++)
			{
				unsigned char r = rgb_buf[i * 3 + 0];
				unsigned char g = rgb_buf[i * 3 + 1];
				unsigned char b = rgb_buf[i * 3 + 2];

				image16_buf[i] = (((r >> 3) & 0x1f) << 11) | 
					(((g >> 2) & 0x3f) << 5) | 
					((b >> 3) & 0x1f);
			}
			break;
		case 24:
			image32_buf = (void *)image_buf;
			for(i = 0;i < pic_pixel;i++)
			{
				unsigned char r = rgb_buf[i * 3 + 0];
				unsigned char g = rgb_buf[i * 3 + 1];
				unsigned char b = rgb_buf[i * 3 + 2];				
				
				image32_buf[i] = (r << 16) | (g << 8) | b;
			}
			break;
	}

	if(x_scr_offs + x_pic > x_scr)
		x_scr_offs = 0;
	if(y_scr_offs + y_pic > y_scr)
		y_scr_offs = 0;

	if(x_pic < x_scr)
		x_min = x_pic;
	else
		x_min = x_scr;

	if(y_pic < y_scr)
		y_min = y_pic;
	else
		y_min = y_scr;

	*fb_buf = (char *) malloc (screen_pixel * cpp + 4);
        
        ((int *)(*fb_buf))[0] = (x_min << 16) | y_min;
	fb_buf_cur = (unsigned char *)(*fb_buf) + 4;

	image_buf_cur = (void *)image_buf;
        memcpy(fb_buf_cur,image_buf_cur, screen_pixel * cpp);

	free(image_buf);
}

static void do_resize(unsigned char *old_buf,int old_x,int old_y, 
		unsigned char *new_buf, int new_x,int new_y)
{
	unsigned char *old_buf_cur;
	unsigned char *new_buf_cur;
	int new_count;
	int old_count;
	int x;
	int y;

	new_buf_cur = new_buf;

	for(y = 0;y < new_y;y++)
	{
		old_buf_cur = old_buf + y * old_y / new_y * old_x * 3;

		new_count = 0;
		for(x = 0;x < new_x;x++)
		{
			old_count = x * old_x / new_x * 3;

			new_buf_cur[new_count + 0] = old_buf_cur[old_count + 0];
			new_buf_cur[new_count + 1] = old_buf_cur[old_count + 1];
			new_buf_cur[new_count + 2] = old_buf_cur[old_count + 2];
			new_count += 3;
		}
		new_buf_cur += new_x * 3;
	}
}

static void picture_resize(unsigned char **rgb_buf,
		int *x_pic,int *y_pic,
		int x_scr,int y_scr)
{
	int x_pic_old = *x_pic;
	int y_pic_old = *y_pic;

	if(x_pic_old > x_scr || y_pic_old > y_scr)
	{
		if(x_pic_old > x_scr)
			*x_pic = x_scr;
		else
			*x_pic = x_pic_old;
		
		if(y_pic_old > y_scr)
			*y_pic = y_scr;
		else
			*y_pic = y_pic_old;

		unsigned char *buf = malloc(*x_pic * *y_pic * 3);

		do_resize(*rgb_buf,x_pic_old,y_pic_old,buf,*x_pic,*y_pic);
		
		free(*rgb_buf);

		*rgb_buf = buf;
	}
}

int show_image(char *filename, char ** buf)
{
	int x_pic = 0;
	int y_pic = 0;
	int x_scr = 0;
	int y_scr = 0;

	int x_scr_offs = 0;
	int y_scr_offs = 0;

	int ret = 0;

	unsigned char *rgb_buf = NULL;

	int (*load)(char *filename,unsigned char *rgb_buf,
			int x_pic,int y_pic);

	if(is_bmp(filename)&&(bmp_getsize(filename,&x_pic,&y_pic) == 0))
	{
		load = bmp_load;
		goto start;
	}

	if(is_jpeg(filename)&&(jpeg_getsize(filename,&x_pic,&y_pic) == 0))
	{
		load = jpeg_load;
		goto start;
	}

	fprintf(stderr,"%s: format unknown.\n",filename);
	return 1;
start:
	if((rgb_buf = malloc(x_pic * y_pic * 3)) == NULL)
	{
		fprintf(stderr,"%s: can't get memory.\n",filename);
		ret = 1;
		goto error;
	}

	if((load(filename,rgb_buf,x_pic,y_pic) != 0))	
	{
		fprintf(stderr,"%s: can't load image.\n",filename);
		ret = 1;
		goto error;
	}
	
	x_scr = X;
	y_scr = Y;
	
	if(x_pic < x_scr)	
		x_scr_offs = (x_scr - x_pic) / 2;
	else
		x_scr_offs = 0;

	if(y_pic < y_scr)	
		y_scr_offs = (y_scr - y_pic) / 2;
	else
		y_scr_offs = 0;

	picture_resize(&rgb_buf,&x_pic,&y_pic,x_scr,y_scr);
	
	mkimage(rgb_buf,x_pic,y_pic,0,0, buf);
		
error:
	return ret;
}


int mk_image_rg888(char * filename, char ** buf)
{

	show_image(filename, buf);
	
	return 0;
}


struct r_jpeg_error_mgr
{
	struct jpeg_error_mgr pub;
	jmp_buf envbuffer;
};


int is_jpeg(char *filename)
{
	int fd;
	unsigned char id[10];

	fd = open(filename,O_RDONLY);
	if(fd==-1) 
		return 0;

	read(fd,id,10);

	close(fd);
	
	if(id[6] == 'J' && id[7] == 'F' && id[8] == 'I' && id[9] == 'F') 
		return 1;

	if(id[0] == 0xff && id[1] == 0xd8 && id[2] == 0xff)
		return 1;

	return 0;
}


void jpeg_cb_error_exit(j_common_ptr cinfo)
{
	struct r_jpeg_error_mgr *mptr;
	mptr = (struct r_jpeg_error_mgr*) cinfo->err;
	cinfo->err->output_message(cinfo);
	longjmp(mptr->envbuffer,1);
}

int jpeg_load(char *filename,unsigned char *buffer, int x,int y)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_decompress_struct *ciptr;
	struct r_jpeg_error_mgr emgr;
	unsigned char *bp;
	int px;
	int py;
	int c;
	FILE *fh;
	JSAMPLE *lb;

	ciptr = &cinfo;

	if(!(fh = fopen(filename,"rb"))) 
		return -1;

	ciptr->err = jpeg_std_error(&emgr.pub);

	emgr.pub.error_exit = jpeg_cb_error_exit;
	if(setjmp(emgr.envbuffer) == 1)
	{
		jpeg_destroy_decompress(ciptr);
		fclose(fh);
		return -1;
	}

	jpeg_create_decompress(ciptr);
	jpeg_stdio_src(ciptr,fh);
	jpeg_read_header(ciptr,TRUE);
	
	ciptr->out_color_space = JCS_RGB;
	
	jpeg_start_decompress(ciptr);

	px = ciptr->output_width; 
	py = ciptr->output_height;
	c = ciptr->output_components;


	if(c == 3)
	{
		lb = ciptr->mem->alloc_small((j_common_ptr)ciptr,JPOOL_PERMANENT, c * px);
		bp = buffer;
		while(ciptr->output_scanline < ciptr->output_height)
		{
			jpeg_read_scanlines(ciptr, &lb, 1);
			memcpy(bp,lb,px * c);
			bp += px * c;
		}					   

	}

	jpeg_finish_decompress(ciptr);
	jpeg_destroy_decompress(ciptr);
	fclose(fh);
	return 0;
}

int jpeg_getsize(char *filename,int *x,int *y)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_decompress_struct *ciptr;
	struct r_jpeg_error_mgr emgr;
	int px,py,c;
	FILE *fh;

	ciptr = &cinfo;
	if(!(fh = fopen(filename,"rb"))) 
		return -1;

	ciptr->err = jpeg_std_error(&emgr.pub);

	emgr.pub.error_exit = jpeg_cb_error_exit;
	if(setjmp(emgr.envbuffer) == 1)
	{
		jpeg_destroy_decompress(ciptr);
		fclose(fh);
		return -1;
	}

	jpeg_create_decompress(ciptr);

	jpeg_stdio_src(ciptr,fh);

	jpeg_read_header(ciptr,TRUE);

	ciptr->out_color_space = JCS_RGB;

	jpeg_start_decompress(ciptr);
	
	px = ciptr->output_width; 
	py = ciptr->output_height;
	c = ciptr->output_components;
	
	*x = px;
	*y = py;

	jpeg_destroy_decompress(ciptr);

	fclose(fh);
	
	return 0;
}


#define BMP_TORASTER_OFFSET	10
#define BMP_SIZE_OFFSET		18
#define BMP_BPP_OFFSET		28


int is_bmp(char *filename)
{
	int fd;
	char id[2] = {0};

	if((fd = open(filename, O_RDONLY)) == -1)
		return 0;

	read(fd, id, 2);

	close(fd);

	if(id[0] == 'B' && id[1] == 'M')
		return 1;

	return 0;
}

int bmp_load(char *filename,unsigned char *buffer,int x_pic,int y_pic)
{
	int fd;
	short bpp;
	int raster;
	int x;
	int y;
	int ret = -1;
	unsigned char buff[4];
	unsigned char *buffer_cur = buffer + x_pic * (y_pic - 1) * 3;

	if((fd = open(filename,O_RDONLY)) == -1)
		return -1;

	if(lseek(fd,BMP_TORASTER_OFFSET,SEEK_SET) == -1)
		goto error;

	read(fd,&raster,4);

	if(lseek(fd, BMP_BPP_OFFSET, SEEK_SET) == -1)
		goto error;

	read(fd, &bpp, 2);

	if(bpp != 24)
		goto error;

	lseek(fd, raster, SEEK_SET);

	for(y = 0; y < y_pic; y++)
	{
		for(x = 0; x < x_pic; x++)
		{
			read(fd, buff, 3);
			*buffer_cur++ = buff[2];
			*buffer_cur++ = buff[1];
			*buffer_cur++ = buff[0];
		}
		buffer_cur -= x_pic * 3 * 2;
	}

	ret = 0;

error:
	
	close(fd);

	return ret;
}

int bmp_getsize(char *filename,int *x_pic,int *y_pic)
{
	int fd;
	int ret = -1;

	if(filename == NULL || x_pic == NULL || y_pic ==  NULL)
		return -1;

	if((fd = open(filename, O_RDONLY)) == -1)
		return -1;

	if(lseek(fd, BMP_SIZE_OFFSET, SEEK_SET) == -1)
		goto error;

	if((read(fd, x_pic, 4)) != 4)
		goto error;

	if((read(fd, y_pic, 4)) != 4)
		goto error;

	ret = 0;
error:

	close(fd);

	return ret;
}
