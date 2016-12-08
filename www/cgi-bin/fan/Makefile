
CC := arm-linux-gcc

all:
	$(CC) -o fan_open_cgi.cgi fan_open.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o fan_close_cgi.cgi fan_close.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
