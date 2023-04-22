#include <linux/module.h>
#include <linux/nmi.h>

#include "../include/header.h"
#include "main.h"
#include "memory.h"
#include "test/test.h"

int jp_main(void *data) {
  for (int i = 0; i < 5; ++i) {
    // touch_nmi_watchdog();
    test_rdtsc(TEST_RDTSC_TIMES);
  }

  pr_info("JudgePenguin: main begin");

  struct judge_penguin_header *header = (struct judge_penguin_header *)kernel_base;
  memset(header->output, 0, sizeof(header->output));
  printk("magic: %08x\n", header->magic);

  u64 entry_addr = (u64)header->entry + kernel_base;
  printk("entry_addr: [v]0x%016llx\n", entry_addr);
  int (*entry)(unsigned int) = (int (*)(unsigned int))entry_addr;
  entry(0);

  printk("magic: %08x\n", header->magic);

  pr_info("JudgePenguin: main end");

  return 0;
}

void cleanup(void) { free_memory(); }