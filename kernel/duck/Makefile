duck_header_files := $(wildcard duck/inc/*.h) $(wildcard duck/inc/*.hpp)

DUCK_CXX := $(CXX) -I duck

include duck/kern/Makefile
include duck/lib/Makefile
include duck/user/Makefile
include duck/user_lib/Makefile