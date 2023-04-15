#include <linux/nmi.h>
#include <linux/module.h> 

#include "main.h"
#include "memory.h"
#include "test/test.h"

int jp_main(void *data) {
  for (int i = 0; i < 10; ++i) {
    // touch_nmi_watchdog();
    test_rdtsc(TEST_RDTSC_TIMES);
  }
  return 0;
}

void cleanup(void) { free_memory(); }