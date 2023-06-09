#include <linux/module.h>
#include <linux/printk.h>

#include "../time.h"
#include "test.h"

#define WARMUP_ROUNDS 2
#define TEST_ROUNDS 5

static void test_init(void *mem, u64 size, u64 step) {
    u64 *buf = (u64 *)mem;
    u64 step_size = step / sizeof(u64);
    u64 buf_size = size / sizeof(u64);

    for (u64 i = 0; i < buf_size; i += step_size)
        buf[i] = (i + step_size < buf_size) ? i + step_size : 0;
}

#define TEST_STEPS (1 << 20)

u64 target;

static u64 test_main(void *mem, u64 size, u64 step) {
    u64 start, end;
    u64 *buf = (u64 *)mem;
    u64 pos = 0, idx = 0;
    start = rdtsc();
    for (; idx < TEST_STEPS; ++idx)
        pos = buf[pos];
    end = rdtsc();
    target ^= pos;
    return end - start;
}

void test_cache(void *mem, u64 size, u64 step) {
    test_init(mem, size, step);
    // use asm to flush dcache
    asm volatile("mfence");

    for (int i = 0; i < WARMUP_ROUNDS; ++i)
        test_main(mem, size, step);

    u64 sum = 0, min = 0xffffffffffffffff, max = 0, tmp;
    for (int i = 0; i < TEST_ROUNDS; ++i) {
        tmp = test_main(mem, size, step);
        if (tmp < min) min = tmp;
        if (tmp > max) max = tmp;
        sum += tmp;
    }
    (void)target;
    pr_info("test cache (size=%lluKiB step=%lluB) avg=%llu min=%llu max=%llu\n", size / 1024, step, sum / TEST_ROUNDS,
            min, max);
}