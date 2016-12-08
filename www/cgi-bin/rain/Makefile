
CC := arm-linux-gcc

all:
	$(CC) -o rain_cgi.cgi rain.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o beep_close_cgi.cgi beep_close.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
