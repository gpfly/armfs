
obj-m	:= demo.o
KERNEL	:= /lib/modules/`uname -r`/build/

all:
	make -C $(KERNEL) M=$(shell pwd)
clean:
	make -C $(KERNEL) M=`pwd` clean


