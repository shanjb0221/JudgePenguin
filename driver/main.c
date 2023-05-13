#include <linux/module.h>
#include <linux/nmi.h>

#include "../include/header.h"
#include "main.h"
#include "memory.h"
#include "test/test.h"

int jpenguin_main(void *data) {
  for (int i = 0; i < 5; ++i) {
    // touch_nmi_watchdog();
    test_rdtsc(TEST_RDTSC_TIMES);
  }

  pr_info("JudgePenguin: main begin");

  struct jpenguin_kernel_header *header =
      (struct jpenguin_kernel_header *)kernel_base;
  memset(header->output, 0, sizeof(header->output));
  printk("magic: %08x\n", header->magic);

  u64 entry_addr = (u64)header->entry + kernel_base;
  printk("entry_addr: [v]0x%016llx\n", entry_addr);
  int (*entry)(unsigned int) = (int (*)(unsigned int))entry_addr;
  int ret = entry(0);

  pr_info("kernel return: %d", ret);

  printk("magic: %08x\n", header->magic);

  for (int i = 0; i < 4; ++i) {
    printk("output[%d] = %08x\n", i, header->output[i]);
  }
  printk("message from kernel: %s\n", (char *)(&header->output[4]));

  pr_info("JudgePenguin: main end");

  return 0;
}
