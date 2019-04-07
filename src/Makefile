obj-m += get_pages.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd) 

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

load:
	sudo insmod get_pages.ko
	sudo dmesg -c

unload:
	sudo rmmod get_pages.ko
	sudo dmesg -c
