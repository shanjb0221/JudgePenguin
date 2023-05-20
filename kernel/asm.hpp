#pragma once

#include "types.h"

struct segment {
  u64 base;
  u16 selector;
};

struct desc_table_reg {
  u16 limit;
  u64 base;
} __attribute__((packed));

static inline void mfence(void) { asm volatile("mfence" : : : "memory"); }

static inline void lfence(void) { asm volatile("lfence" : : : "memory"); }

static u64 __force_order;

static inline u64 rcr0(void) {
  u64 cr0;
  asm volatile("mov %%cr0, %0" : "=r"(cr0), "=m"(__force_order));
  return cr0;
}
static inline void wcr0(u64 cr0) {
  asm volatile("mov %0, %%cr0" : : "r"(cr0), "m"(__force_order));
}

static inline u64 rcr2(void) {
  u64 cr2;
  asm volatile("mov %%cr2, %0" : "=r"(cr2), "=m"(__force_order));
  return cr2;
}
static inline void wcr2(u64 cr2) {
  asm volatile("mov %0, %%cr2" : : "r"(cr2), "m"(__force_order));
}

static inline u64 rcr3(void) {
  u64 cr3;
  asm volatile("mov %%cr3, %0" : "=r"(cr3), "=m"(__force_order));
  return cr3;
}
static inline void wcr3(u64 cr3) {
  asm volatile("mov %0, %%cr3" : : "r"(cr3), "m"(__force_order));
}

static inline u64 rcr4(void) {
  u64 cr4;
  asm volatile("mov %%cr4, %0" : "=r"(cr4), "=m"(__force_order));
  return cr4;
}
static inline void wcr4(u64 cr4) {
  asm volatile("mov %0, %%cr4" : : "r"(cr4), "m"(__force_order));
}

static inline void sgdtq(struct desc_table_reg *dtr) {
  asm volatile("sgdtq %0" : "=m"(*dtr));
}
static inline void lgdtq(struct desc_table_reg *dtr) {
  asm volatile("lgdtq %0" : : "m"(*dtr));
}

static inline void sidtq(struct desc_table_reg *dtr) {
  asm volatile("sidtq %0" : "=m"(*dtr));
}
static inline void lidtq(struct desc_table_reg *dtr) {
  asm volatile("lidtq %0" : : "m"(*dtr));
}

#define NUM_GDT_DESC 16

#define GDT_DESC_NULL 0
#define GDT_DESC_CODE 1
#define GDT_DESC_TSS 2
#define GDT_DESC_TSS_HI 3

#define DESC_TSS_BUSY (1UL << (9 + 32))
#define DESC_PRESENT (1UL << (15 + 32))
#define DESC_CODE_DATA (1UL << (12 + 32))
#define DESC_PAGE_GRAN (1UL << (23 + 32))

#define MSR_EFER 0xc0000080
#define MSR_STAR 0xc0000081
#define MSR_LSTAR 0xc0000082
#define MSR_CSTAR 0xc0000083
#define MSR_SFMASK 0xc0000084
#define MSR_FS_BASE 0xc0000100
#define MSR_GS_BASE 0xc0000101
#define MSR_KERNGS_BASE 0xc0000102

static inline u64 rdmsr(u32 msr) {
  u32 lo, hi;
  asm volatile("rdmsr" : "=a"(lo), "=d"(hi) : "c"(msr));
  return lo | ((u64)hi << 32);
}

static inline void wrmsr(u32 msr, u64 val) {
  asm volatile("wrmsr" : : "a"(val), "d"(val >> 32), "c"(msr) : "memory");
}

static inline void set_cs(u16 selector) {
  asm volatile("lea 1f(%%rip),%%rax\n\t"
               "push %0\n\t"
               "push %%rax\n\t"
               "lretq\n\t"
               "1:"
               :
               : "m"(selector)
               : "rax");
}