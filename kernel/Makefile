target := build/kernel.bin

CXX := g++
CXX += -std=c++14
CXX += -g -fPIC
CXX += -Wall -Wextra -Werror -march=x86-64
CXX += -fno-strict-aliasing  # prevent ub!
# TODO: we should initialize fs for stack canary, maybe use libc
CXX += -fno-stack-protector # remove stack canary XD
CXX += -mno-red-zone
CXX += -U_FORTIFY_SOURCE

linker_script := linker.ld

LIB_PREFIX := duck-lib64/
LIBC_PREFIX := $(LIB_PREFIX)musl/
libstdcxx_files := $(LIB_PREFIX)libstdc++.a
libc_files := -L $(LIBC_PREFIX) -L $(LIB_PREFIX) -lc -lgcc -lgcc_eh -lc
libc_crt_start := $(LIBC_PREFIX)crt1.o $(LIBC_PREFIX)crti.o
libc_crt_end := $(LIBC_PREFIX)crtn.o

all: $(target) locate_entry install

include duck/Makefile
include wrapper/Makefile

.PHONY: all clean install

clean:
	@rm -rf build/*

build/kernel.elf: $(kern_object_files) $(lib_object_files) $(wrapper_object_files) $(linker_script) \
	$(libc_duck64) $(kern_asm_object_files) $(wrapper_asm_object_files) $(user_obj_files) $(user32_obj_files)
	@echo + ld $@
	@ld -n -pie -T $(linker_script) -o $@ \
		$(libc_crt_start) \
		$(kern_asm_object_files) $(kern_object_files) \
		$(wrapper_asm_object_files) $(wrapper_object_files) \
		$(lib_object_files) $(libstdcxx_files) $(libc_files) $(libc_crt_end) \
		$(user_obj_files) $(user32_obj_files)

# build/kernel.out.strip: build/kernel.out
# 	@echo + strip $@
# 	@cp $< $@
# 	@strip --strip-all $@

$(target): build/kernel.elf
	@echo + objcopy $@
	@objcopy -O binary $< $@

install: $(target)
	@echo + install $<
	@sudo cp $< /lib/firmware/jpenguin_kernel.bin

gdb: build/remote.elf
	gdb -q -d . \
	  -ex 'target remote localhost:1234' \
	  -ex 'symbol-file build/remote.elf -o 0xffffc90040136000' \
	  -ex 'b entry' \
	  -ex 'c'

locate_%: build/kernel.elf
	@nm build/kernel.elf | sort | c++filt | grep $*

locate_remote_%: build/remote.elf
	@nm build/remote.elf | sort | c++filt | grep $*

build/%.asm: build/%.elf
	@objdump -d build/$*.elf > build/$*.asm