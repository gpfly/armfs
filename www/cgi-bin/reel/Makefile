
CC := arm-linux-gcc

all:
	$(CC) -o reel_zheng_open_cgi.cgi reel_zheng_open.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o reel_fan_open_cgi.cgi reel_fan_open.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o reel_stop_cgi.cgi reel_stop.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
