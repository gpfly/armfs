
obj-m	:= demo.o

KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=`pwd`
install:
	make -C $(KERNEL) M=`pwd` modules_install
clean:
	make -C $(KERNEL) M=`pwd` clean


