#include <cstdint>

#include "header.hpp"

extern "C" int entry(uint64_t);

jpenguin_kernel_header __attribute__((section(".header")))
header{.signature = {'J', 'P', 'e', 'n', 'g', 'u', 'i', 'n'},
       .entry = entry,
       .magic = int(0xdeadbeef),
       .vp_addr_diff = 0,
       .output = {0}};