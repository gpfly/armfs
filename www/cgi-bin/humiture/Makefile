
CC := arm-linux-gcc

all:
	$(CC) -o humiture_cgi.cgi humiture.c -lcgi -L../cgilib/ -I../include/
	$(CC) -o fan_close_cgi.cgi fan_close.c -lcgi -L../cgilib/ -I../include/
clean:
	rm -f *.cgi
	
