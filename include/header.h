#ifndef JP_HEADER_H
#define JP_HEADER_H

typedef signed char s8;
typedef unsigned char u8;

typedef signed short s16;
typedef unsigned short u16;

typedef signed int s32;
typedef unsigned int u32;

typedef signed long long s64;
typedef unsigned long long u64;

struct segment {
  u64 base;
  u32 limit;
  u32 access_rights;
  u16 selector;
};

struct desc_table_reg {
  u16 limit;
  u64 base;
} __attribute__((packed));

struct linux_data {
  u64 sp, ip, cr0, cr3, cr4, efer;
  struct desc_table_reg gdtr, idtr;
  struct segment cs, ds, es, fs, gs, tss;
};

struct judge_penguin_header {
  char signature[8];
  int (*entry)(unsigned int);
  // struct linux_data linux_data;
  int magic;
  int output[100];
} __attribute__((packed));

#endif