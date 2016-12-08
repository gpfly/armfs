CC=arm-linux-gcc
INCLUDE_PATH=/usr/local/include/
LIB_PATH=/usr/local/lib/

vedio_test : main.c libjpeg.a
	$(CC) -ljpeg -o $@ $^ 
#$(CC) -L$(LIB_PATH) -I$(INCLUDE_PATH) -ljpeg -o $@ $^ 

clean :
	rm  vedio_test 
