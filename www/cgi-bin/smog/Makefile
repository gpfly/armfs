
CC := arm-linux-gcc

all:
	$(CC) -o smog_cgi.cgi smog.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o smog_close_cgi.cgi smog_close.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
