CC := g++
CC += -Wall -Wextra
CC += -fno-stack-protector -fno-strict-aliasing -fno-omit-frame-pointer
CC += -mno-red-zone

LD := ld

OBJS := $(patsubst %.cpp, %.o, $(wildcard *.cpp))
OBJS += $(patsubst %.S, %.o, $(wildcard *.S))

all: install

%.o: %.cpp
	$(CC) -c -o $@ $<

%.o: %.S
	$(CC) -c -o $@ $<

kernel: $(OBJS)
	$(LD) -n -T linker.ld -o $@ $^ -nostdlib

kernel.bin: kernel
	objcopy -O binary $< $@

install: kernel.bin
	sudo cp kernel.bin /lib/firmware/jpenguin_kernel.bin

clean:
	rm -f *.o kernel kernel.bin