#include <linux/module.h>
#include <linux/nmi.h>

#include "../include/header.h"
#include "main.h"
#include "memory.h"
#include "test/test.h"

int jpenguin_main(void *data) {
  for (int i = 0; i < 1; ++i) {
    // touch_nmi_watchdog();
    test_rdtsc(TEST_RDTSC_TIMES);
  }

  pr_info("JudgePenguin: main begin");

  struct jpenguin_kernel_header *header =
      (struct jpenguin_kernel_header *)kernel_base;
  header->vp_addr_diff = vp_addr_diff;
  pr_info("vp_addr_diff: 0x%016llx\n", header->vp_addr_diff);
  memset(header->output, 0, sizeof(header->output));
  pr_info("magic: %08x\n", header->magic);
  for (int i = 0; i < 4; ++i) {
    pr_info("output[%d] = %08x\n", i, header->output[i]);
  }

  u64 entry_addr = (u64)header->entry + kernel_base;
  pr_info("entry addr: [v]0x%016llx\n", entry_addr);
  int (*entry)(u64) = (int (*)(u64))entry_addr;
  int ret = entry(kernel_stack_top);

  pr_info("kernel return: %d", ret);

  pr_info("magic: %08x\n", header->magic);

  for (int i = 0; i < 4; ++i) {
    pr_info("output[%d] = %08x\n", i, header->output[i]);
  }
  pr_info("message from kernel: %s\n", (char *)(&header->output[4]));

  pr_info("JudgePenguin: main end");

  return 0;
}
