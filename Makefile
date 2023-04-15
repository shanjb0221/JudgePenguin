obj-m += test.o
test-objs := entry.o main.o
 
PWD := $(CURDIR) 

CONFIG_MODULE_SIG=n
 
all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo insmod test.ko

remove:
	sudo rmmod test

reload: remove load

ps:
	sudo journalctl --since "5 minutes ago" | grep -i kernel