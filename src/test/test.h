#ifndef JP_TEST_H
#define JP_TEST_H

#include "../common.h"

extern const uint TEST_RDTSC_TIMES;

extern void test_rdtsc(uint rounds);
extern void test_memory(void *mem, u64 size);

#endif
