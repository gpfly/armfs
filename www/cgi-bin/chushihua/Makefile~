
CC := arm-linux-gcc

all:
	$(CC) -o led_open_cgi.cgi led_open.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o led_close_cgi.cgi led_close.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
