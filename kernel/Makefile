target := build/kernel.bin

CXX := g++
CXX += -std=c++14
CXX += -Wall -Wextra -Werror -march=x86-64
CXX += -fno-strict-aliasing  # prevent ub!
CXX += -mno-red-zone
CXX += -U_FORTIFY_SOURCE

linker_script := linker.ld

LIB_PREFIX := duck-lib64/
LIBC_PREFIX := $(LIB_PREFIX)musl/
libstdcxx_files := $(LIB_PREFIX)libstdc++.a
libc_files := -L $(LIBC_PREFIX) -L $(LIB_PREFIX) -lc -lgcc -lgcc_eh -lc
libc_crt_start := $(LIBC_PREFIX)crt1.o $(LIBC_PREFIX)crti.o
libc_crt_end := $(LIBC_PREFIX)crtn.o

all: $(target) install

include duck/Makefile
include wrapper/Makefile

.PHONY: all clean install

clean:
	@rm -r build/*

build/kernel.out: $(kern_object_files) $(lib_object_files) $(wrapper_object_files) $(linker_script) \
	$(libc_duck64) $(kern_asm_object_files) $(wrapper_asm_object_files)
	@echo + ld $@
	@ld -n -T $(linker_script) -o $@ \
		$(libc_crt_start) \
		$(kern_asm_object_files) $(kern_object_files) \
		$(wrapper_asm_object_files) $(wrapper_object_files) \
		$(lib_object_files) $(libstdcxx_files) $(libc_files) $(libc_crt_end)

# build/kernel.out.strip: build/kernel.out
# 	@echo + strip $@
# 	@cp $< $@
# 	@strip --strip-all $@

$(target): build/kernel.out
	@echo + objcopy $@
	@objcopy -O binary $< $@

install: $(target)
	@echo + install $<
	@sudo cp $< /lib/firmware/jpenguin_kernel.bin