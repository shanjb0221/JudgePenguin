#include <linux/module.h> /* Needed by all modules */
#include <linux/nmi.h>
#include <linux/printk.h> /* Needed for pr_info() */

#include "device.h"
#include "main.h"
#include "memory.h"
#include "firmware.h"
#include "test/test.h"

static atomic_t call_done;

static void each_cpu_main(void *data) {
  uint cpu = smp_processor_id();
  uint cpus = num_online_cpus();
  unsigned long flag;
  int ret;

  pr_info("disable interrupt on cpu %d.\n", cpu);

  local_irq_save(flag);

  atomic_inc(&call_done);
  while (atomic_read(&call_done) != cpus)
    cpu_relax();

  touch_nmi_watchdog();

  // watchdog_nmi_disable(cpu);

  if (cpu) {
    while (atomic_read(&call_done) != 0)
      cpu_relax();
  } else {
    ret = jpenguin_main(data);
    pr_info("Leaving Steady Mode.\n");
    atomic_set(&call_done, 0);
  }

  // watchdog_nmi_enable(cpu);

  local_irq_restore(flag);

  pr_info("restore interrupt on cpu %d.\n", cpu);
}

int init_module(void) {
  uint online_cpus;

  pr_info("Hello world from Judge Penguin.\n");
  pr_info("Initializing Judge Penguin.\n");

  online_cpus = num_online_cpus();
  pr_info("online_cpus: %d\n", online_cpus);

  test_rdtsc(TEST_RDTSC_TIMES);

  init_device();
  init_memory();
  load_firmware();

  pr_info("Entering Steady Mode.\n");
  atomic_set(&call_done, 0);
  on_each_cpu(each_cpu_main, NULL, 0);

  /* A non 0 return means init_module failed; module can't be loaded. */
  return 0;
}

void cleanup_module(void) {
  pr_info("Cleaning up Judge Penguin.\n");
  free_memory();
  exit_device();
  pr_info("Goodbye world from Judge Penguin.\n");
}

MODULE_LICENSE("GPL");
