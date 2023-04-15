#include <linux/printk.h>

#include "../common.h"
#include "test.h"

void test_memory(void *mem, u64 size) {
  pr_info("test_memory mem=0x%pK size=0x%llx.\n", mem, size);
  int pass = 1;

  // test mem
  for (int i = 0, prev, curv; i < 5; i++) {
    prev = ((int *)mem)[i];
    __asm__ volatile("" : : : "memory");
    ((int *)mem)[i]++;
    __asm__ volatile("" : : : "memory");
    curv = ((int *)mem)[i];
    pr_info("mem[0x%x]: %d -> %d", i, prev, curv);
    if (prev + 1 != curv)
      pass = 0;
  }

  for (int i = size / 4 - 5, prev, curv; i < (int)size / 4; i++) {
    prev = ((int *)mem)[i];
    __asm__ volatile("" : : : "memory");
    ((int *)mem)[i]++;
    __asm__ volatile("" : : : "memory");
    curv = ((int *)mem)[i];
    pr_info("mem[0x%x]: %d -> %d", i, prev, curv);
    if (prev + 1 != curv)
      pass = 0;
  }

  if (pass)
    pr_info("test_memory pass.\n");
  else
    pr_err("test_memory fail!\n");
}