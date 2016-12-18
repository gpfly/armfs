
obj-m	:= demo.o fool.o

KERNEL	:= /lib/modules/4.5.5-300.fc24.i686+PAE/build/
#KERNEL	:= /linux-3.5/

all:
	make -C $(KERNEL) M=`pwd`
install:
	make -C $(KERNEL) M=`pwd` modules_install
clean:
	make -C $(KERNEL) M=`pwd` clean


