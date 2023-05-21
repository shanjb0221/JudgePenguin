kern_source_files := $(wildcard duck/kern/*.cpp)
kern_object_files := $(patsubst duck/kern/%.cpp, \
		build/duck/kern/%.o, $(kern_source_files))

kern_asm_source_files := $(wildcard duck/kern/*.S)
kern_asm_object_files := $(patsubst duck/kern/%.S, \
		build/duck/kern/%.o, $(kern_asm_source_files))

KERN_CXX := $(DUCK_CXX) -O2

build/duck/kern/%.o: duck/kern/%.cpp $(duck_header_files)
	@echo + cxx $@
	@mkdir -p $(shell dirname $@)
	@$(KERN_CXX) -c $< -o $@

build/duck/kern/%.o: duck/kern/%.S
	@echo + as $@
	@mkdir -p $(shell dirname $@)
	@$(DUCK_CXX) -c $< -o $@