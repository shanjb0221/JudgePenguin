CC := gcc
CC += -Wall -Wextra
CC += -fno-stack-protector -fno-strict-aliasing -fno-omit-frame-pointer
CC += -mno-red-zone

LD := ld

OBJS := $(patsubst %.c, %.o, $(wildcard *.c))
OBJS += $(patsubst %.S, %.o, $(wildcard *.S))

all: kernel.bin

%.o: %.c
	$(CC) -c -o $@ $<

%.o: %.S
	$(CC) -c -o $@ $<

kernel: $(OBJS)
	$(LD) -n -T linker.ld -o $@ $^ -nostdlib

kernel.bin: kernel
	objcopy -O binary $< $@

clean:
	rm -f *.o kernel kernel.bin