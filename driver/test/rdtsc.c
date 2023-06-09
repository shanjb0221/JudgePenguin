#include <linux/module.h>
#include <linux/printk.h>

#include "../time.h"
#include "test.h"

const uint TEST_RDTSC_TIMES = 100000000u;

void test_rdtsc(uint rounds) {
    pr_info("test rdtsc on cpu %d for %u rounds.\n", smp_processor_id(), rounds);
    u64 prev, cur, delta, sum, max_;
    uint bad_count = 0;

    prev = rdtsc();
    sum = max_ = 0;
    for (uint i = 0; i < rounds; ++i) {
        cur = rdtsc();
        delta = cur - prev;
        sum += delta;
        if (delta > max_) max_ = delta;
        if (delta > 100) bad_count++;
        prev = cur;
    }

    pr_info("avg=%llu max=%llu bad_count=%d.\n", sum / rounds, max_, bad_count);
    if (bad_count == 0)
        pr_info("test rdtsc pass.\n");
    else if (max_ < 10000)
        pr_warn("test rdtsc fail, but not serious.\n");
    else
        pr_err("test rdtsc fail!\n");
}