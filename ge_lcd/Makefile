TARGET			:= lcd_disp
Sys			:= System.map

OBJS			:= ./src/main.o  ./src/ge_lcd.o
OBJS 			+= ./src/ge_list.o
OBJS 			+= ./src/number.o
OBJS 			+= ./src/mkimage.o
OBJS 			+= ./src/ts.o
OBJS 			+= ./src/watch_data.o
OBJS 			+= ./src/camera.o
OBJS 			+= ./src/wangfan.o ./src/wangirr.o \
			   ./src/wangled.o ./src/wangred.o
OBJS 			+= ./src/beep.o ./src/smogapp.o \
			   ./src/rainapp.o ./src/humidityapp.o \
			   ./src/zhaoapp.o

OBJS 			+= ./src/bjdj.o

CROSS_COMPILE		:= arm-linux-
CC			:= $(CROSS_COMPILE)gcc
OBJCOPY			:= $(CROSS_COMPILE)objcopy
LD			:= $(CROSS_COMPILE)ld
NM			:= $(CROSS_COMPILE)nm
OBJDUMP			:= $(CROSS_COMPILE)objdump

CFLAGS			:= -I./include -Wall

$(Sys):$(TARGET)
	$(NM) $< > $@

$(TARGET):$(OBJS)
	$(CC) $^ -o $@ -ljpeg -lpthread

./src/mkimage.o:./src/mkimage.c
	$(CC) -c $< -o $@ $(CFLAGS)  

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY:clean
clean:
	rm ./src/*.o *.map $(TARGET) -f
new:
	make clean;make
