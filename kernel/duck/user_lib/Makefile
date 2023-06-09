user_lib_header_files := $(wildcard duck/user_lib/*.hpp)
user_lib_source_files := $(wildcard duck/user_lib/*.cpp)
user_lib_asm_source_files := $(wildcard duck/user_lib/*.S)
user_lib_object_files := $(patsubst duck/user_lib/%.cpp, \
		build/duck/user_lib/%.o, $(user_lib_source_files))

user_lib_object_files += $(patsubst duck/user_lib/%.S, \
		build/duck/user_lib/%.o, $(user_lib_asm_source_files))

USER_LIB_CXX := $(DUCK_CXX) -O2 -static -nostdlib -U_FORTIFY_SOURCE

build/duck/user_lib/%.o: duck/user_lib/%.cpp $(user_lib_header_files)
	@echo + cxx $@
	@mkdir -p $(shell dirname $@)
	@$(USER_LIB_CXX) -c $< -o $@

build/duck/user_lib/%.o: duck/user_lib/%.S $(user_lib_header_files)
	@echo + as $@
	@mkdir -p $(shell dirname $@)
	@$(USER_LIB_CXX) -c $< -o $@

build/duck/user_lib/libduck.a: $(user_lib_object_files)
	@echo + ar $@
	@mkdir -p $(shell dirname $@)
	@ar r $@ $(user_lib_object_files)
