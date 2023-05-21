wrapper_source_files := $(wildcard wrapper/*.cpp)
wrapper_object_files := $(patsubst wrapper/%.cpp, \
		build/wrapper/%.o, $(wrapper_source_files))

wrapper_header_files := $(wildcard wrapper/*.h) $(wildcard wrapper/*.hpp)

wrapper_asm_source_files := $(wildcard wrapper/*.S)
wrapper_asm_object_files := $(patsubst wrapper/%.S, \
		build/wrapper/%.o, $(wrapper_asm_source_files))

WRAPPER_CXX := $(CXX) -O2

build/wrapper/%.o: wrapper/%.cpp $(wrapper_header_files)
	@echo + cxx $@
	@mkdir -p $(shell dirname $@)
	@$(WRAPPER_CXX) -c $< -o $@

build/wrapper/%.o: wrapper/%.S
	@echo + as $@
	@mkdir -p $(shell dirname $@)
	@$(WRAPPER_CXX) -c $< -o $@