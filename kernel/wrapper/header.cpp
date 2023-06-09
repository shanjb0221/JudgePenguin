#include <cstdint>

#include "header.hpp"

extern "C" int entry(uint64_t);

jpenguin_kernel_header __attribute__((section(".header"))) header{
    .signature = {'J', 'P', 'e', 'n', 'g', 'u', 'i', 'n'},
    .entry = entry,
    .kernel_base = 0x0,
    .kernel_stack_top = 0x0,
    .magic = int(0xdeadbeef),
};