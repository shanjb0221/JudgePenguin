#include <linux/module.h> /* Needed by all modules */
#include <linux/nmi.h>
#include <linux/printk.h> /* Needed for pr_info() */

#include "main.h"

typedef unsigned long long time_t;

static inline time_t get_time(void) {
  uint32_t hi, lo;
  __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi) : :);
  return (time_t)hi << 32 | lo;
}

void test_time(uint rounds) {
  time_t prev, cur, delta, sum, max_;
  uint bad_count = 0;
  uint i;

  prev = get_time();
  sum = 0;
  max_ = 0;
  for (i = 0; i < rounds; ++i) {
    cur = get_time();
    delta = cur - prev;
    sum += delta;
    if (delta > max_)
      max_ = delta;
    if (delta > 100)
      bad_count++;
    prev = cur;
  }

  pr_info("test time on cpu %d avg:%llu max:%llu bad_count:%d\n",
          smp_processor_id(), sum / rounds, max_, bad_count);
}

int main(void *data) {
  int i;
  for (i = 0; i < 30; ++i) {
    // touch_nmi_watchdog();
    test_time(RDTSC_TIMES);
  }
  return 0;
}

MODULE_LICENSE("GPL");