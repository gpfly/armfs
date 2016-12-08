
CC := arm-linux-gcc

all:
	$(CC) -o camera_cgi.cgi camera_cgi.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o cameraclose_cgi.cgi cameraclose_cgi.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o video_cgi.cgi video_cgi.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
