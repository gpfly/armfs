
obj-m	:= xf.o
#xf-objs	:= fool.o demo.o
xf-y	:= fool.o demo.o

KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=`pwd`
clean:
	make -C $(KERNEL) M=`pwd` clean


