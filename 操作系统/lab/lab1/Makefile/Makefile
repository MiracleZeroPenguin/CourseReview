TARGET=system_traversal
KDIR=/usr/src/linux
PWD=$(shell pwd)
obj-m +=$(TARGET).o
default:
	make -C $(KDIR) M=$(PWD) modules
