#include "time.h"
#include "common.h"

u64 rdtsc() {
  u32 hi, lo;
  __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi) : :);
  return (u64)hi << 32 | lo;
}
