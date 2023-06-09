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

    struct jpenguin_kernel_header *header = (struct jpenguin_kernel_header *)(kernel_base_linux + KERNEL_OFFSET);
    pr_debug("header signature: %.8s\n", header->signature);
    pr_info("header magic: %08x\n", header->magic);

    u64 entry_addr = (u64)header->entry + kernel_base;
    pr_err("entry addr: [v]0x%016llx\n", entry_addr);
    int (*entry)(u64) = (int (*)(u64))entry_addr;

    pr_info("now call kernel entry with kernel stack top: [vL]0x%016llx\n", kernel_stack_top_linux);
    int ret = entry(kernel_stack_top_linux);

    pr_info("kernel exit with code %d", ret);
    pr_info("header magic: %08x\n", header->magic);
    pr_info("JudgePenguin: main end");

    return 0;
}
