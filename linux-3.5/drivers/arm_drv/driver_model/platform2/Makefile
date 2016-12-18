
obj-m	:= device1.o device2.o driver.o
 
KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	make -C $(KERNEL) M=`pwd` clean


