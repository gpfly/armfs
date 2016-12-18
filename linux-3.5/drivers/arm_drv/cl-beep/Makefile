
obj-y	:= beep.o
KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	make -C $(KERNEL) M=`pwd` clean
