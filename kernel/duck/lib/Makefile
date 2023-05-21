lib_source_files := $(wildcard duck/lib/*.cpp)
lib_object_files := $(patsubst duck/lib/%.cpp, \
		build/duck/lib/%.o, $(lib_source_files))

LIB_CXX := $(DUCK_CXX) -O2

build/duck/lib/%.o: duck/lib/%.cpp $(duck_header_files)
	@echo + cxx $@
	@mkdir -p $(shell dirname $@)
	@$(LIB_CXX) -c $< -o $@
