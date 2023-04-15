obj-m += JudgePenguin.o
# test-objs := $(patsubst %.c,%.o,$(wildcard *.c ./test/*.c))
test-objs := test/memory.o test/rdtsc.o
JudgePenguin-objs := $(test-objs) main.o entry.o memory.o time.o
# judgepenguin-objs += $(test-objs)

ccflags-y := -std=gnu99 -Wno-declaration-after-statement -O0
 
PWD := $(CURDIR) 

CONFIG_MODULE_SIG=n
 
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 
 
clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load:
	sudo sync && sudo insmod JudgePenguin.ko

remove:
	sudo sync && sudo rmmod JudgePenguin

reload: remove load

trace:
	sudo journalctl --since "1 minute ago" | grep -i kernel