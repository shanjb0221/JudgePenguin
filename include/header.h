#ifndef JP_HEADER_H
#define JP_HEADER_H

#ifndef __KERNEL__
#include <stdint.h>
#else
typedef unsigned long long uint64_t;
#endif

struct jpenguin_kernel_header {
  char signature[8];
  int (*entry)(uint64_t);
  int magic;
  uint64_t vp_addr_diff;
  int output[4];
} __attribute__((packed));

#endif