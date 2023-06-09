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
  uint64_t kernel_base;      // virtual address of kernel (not offseted, in JPenguin low address space)
  uint64_t kernel_stack_top; // virtual address of kernel stack top (not offseted, in JPenguin low address space)
  int magic;
} __attribute__((packed));

#ifndef __KERNEL__
extern struct jpenguin_kernel_header header;

inline uintptr_t virt_base() { return header.kernel_base; }
inline uintptr_t virt2phys(uintptr_t virt) { return virt - virt_base() + PHYS_BASE; }
#endif

#endif