#include "../include/header.h"

#include "asm.h"

extern int entry(unsigned int);

struct judge_penguin_header __attribute__((section(".header")))
header = {.signature = "JPenguin", .entry = entry, .magic = 0xdeadbeef};

#define NUM_ENTRY_REGS 6

struct linux_data {
  u64 sp, ip, cr0, cr3, cr4, efer;
  u64 reg[NUM_ENTRY_REGS];
  struct desc_table_reg gdtr, idtr;
  struct segment cs, ds, es, fs, gs, tss;
};

static struct linux_data data;

static u64 gdt[NUM_GDT_DESC] = {
    [GDT_DESC_NULL] = 0,
    [GDT_DESC_CODE] = 0x00af9b000000ffffUL,
    [GDT_DESC_TSS] = 0x0000890000000000UL,
    [GDT_DESC_TSS_HI] = 0x0000000000000000UL,
};

#define CR3_PHYS 0x40000000llu

int save_linux() {
  struct desc_table_reg dtr;

  // save GDTR
  sgdtq(&data.gdtr);

  // save TR and TSS descriptor
  asm volatile("str %0" : "=m"(data.tss.selector));
  read_descriptor(&data.tss, &data.gdtr);
  // if (data.tss.selector / 8 >= 16)
  //   return -1;

  // save CS
  asm volatile("mov %%cs, %0" : "=r"(data.cs.selector));
  read_descriptor(&data.cs, &data.gdtr);

  // save DS, ES, FS, GS
  asm volatile("mov %%ds, %0" : "=r"(data.ds.selector));
  read_descriptor(&data.ds, &data.gdtr);
  asm volatile("mov %%es, %0" : "=r"(data.es.selector));
  read_descriptor(&data.es, &data.gdtr);
  asm volatile("mov %%fs, %0" : "=r"(data.fs.selector));
  read_descriptor(&data.fs, &data.gdtr);
  data.fs.base = rdmsr(MSR_FS_BASE);
  asm volatile("mov %%gs, %0" : "=r"(data.gs.selector));
  read_descriptor(&data.gs, &data.gdtr);
  data.gs.base = rdmsr(MSR_GS_BASE);

  // read registers to restore (?)
  // for (u32 n = 0; n < NUM_ENTRY_REGS; n++)
  //   data.reg[n] = ((u64 *)(&data.sp))[n];
  // data.ip = ((u64 *)(&data.sp))[NUM_ENTRY_REGS];

  // set GDT
  dtr.limit = NUM_GDT_DESC * 8 - 1;
  dtr.base = (u64)&gdt;
  lgdtq(&dtr);

  // set CS
  set_cs(GDT_DESC_CODE * 8);

  // TODO: swap IDTR

  // clear DS, ES, SS
  asm volatile("mov %0, %%ds" : : "r"(0));
  asm volatile("mov %0, %%es" : : "r"(0));
  asm volatile("mov %0, %%ss" : : "r"(0));

  // // clear TSS busy flag, set TR
  gdt[GDT_DESC_TSS] &= ~DESC_TSS_BUSY;
  asm volatile("ltr %%ax" : : "a"(GDT_DESC_TSS * 8));

  // save CR0, CR4
  data.cr0 = rcr0();
  data.cr4 = rcr4();

  // swap CR3!!
  data.cr3 = rcr3();

  u64 pc;
  // load current pc
  asm volatile("lea 0(%%rip), %0" : "=r"(pc));
  header.output[0] = pc;
  header.output[1] = pc >> 32;
  header.output[2] = data.cr3;
  header.output[3] = data.cr3 >> 32;

  wcr3(CR3_PHYS);
  return 0;
}

void restore_linux() {
  // restore CR0, CR4
  wcr0(data.cr0);
  wcr4(data.cr4);

  // restore CR3
  wcr3(data.cr3);

  // restore GDTR
  u64 *linux_gdt = (u64 *)data.gdtr.base;
  u32 tss_idx = data.tss.selector / 8;

  gdt[tss_idx] = linux_gdt[tss_idx] & ~DESC_TSS_BUSY;
  gdt[tss_idx + 1] = linux_gdt[tss_idx + 1];
  asm volatile("ltr %%ax" : : "a"(data.tss.selector));

  lgdtq(&data.gdtr);
  // // TODO: restore IDTR

  // // restore CS
  set_cs(data.cs.selector);

  // restore DS, ES, FS
  asm volatile("mov %0, %%ds" : : "r"(data.ds.selector));
  asm volatile("mov %0, %%es" : : "r"(data.es.selector));
  asm volatile("mov %0, %%fs" : : "r"(data.fs.selector));
  asm volatile("swapgs\n\t"
               "mov %0, %%gs\n\t"
               "mfence\n\t"
               "swapgs" ::"r"(data.gs.selector));

  wrmsr(MSR_FS_BASE, data.fs.base);
  wrmsr(MSR_GS_BASE, data.gs.base);
}

int main(void) {
  header.magic++;

  int ret = save_linux();
  if (ret)
    return ret;

  header.magic++;

  char *p = (char *)0x109024ull; // magic address in JPenguin mode
  char *q = "Hello, world!";
  for (int i = 0; i < 13; i++) {
    p[i] = q[i];
  }
  p[13] = '\0';

  restore_linux();
  return 0;
}