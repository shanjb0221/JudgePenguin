obj-m += JudgePenguin.o
test-objs := test/memory.o test/rdtsc.o
JudgePenguin-objs := $(test-objs) main.o entry.o
JudgePenguin-objs += memory.o time.o page_table.o device.o firmware.o

ccflags-y := -std=gnu11 -Wno-declaration-after-statement -g
 
PWD := $(CURDIR) 

CONFIG_MODULE_SIG=n
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: killdog
	sudo sync && sudo insmod ./JudgePenguin.ko

remove:
	sudo sync && sudo rmmod JudgePenguin
	@sudo sysctl -w kernel.watchdog=1 

reload: remove load

trace:
	sudo journalctl --since "1 minute ago" | grep -i kernel

killdog:
	@sudo sysctl -w kernel.watchdog=0