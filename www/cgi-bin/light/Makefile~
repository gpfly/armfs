
CC := arm-linux-gcc

all:
	$(CC) -o door_open_cgi.cgi door_open.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o door_close_cgi.cgi door_close.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o door_stop_cgi.cgi door_stop.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
