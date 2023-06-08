#ifndef JP_HEADER_H
#define JP_HEADER_H

#ifndef __KERNEL__
#include <stdint.h>
#else
typedef unsigned long long uint64_t;
#endif

#define KERNEL_OFFSET 0x160 // get it from elf header, by make -C kernel locate_header
#define PHYS_BASE 0x40000000llu

struct jpenguin_kernel_header {
  char signature[8];
  int (*entry)(uint64_t);
  int magic;
  uint64_t vp_addr_diff;
  int output[4];
} __attribute__((packed));

#endif