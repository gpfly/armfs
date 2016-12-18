
obj-m	:= bus.o device1.o driver.o
obj-m	+= device2.o device3.o device4.o
 
KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	make -C $(KERNEL) M=`pwd` clean


