
CC := arm-linux-gcc

all:
	$(CC) -o irr_close_cgi.cgi irr_open.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o irr_open_cgi.cgi irr_close.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
