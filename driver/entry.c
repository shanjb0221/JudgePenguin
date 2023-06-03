#include <linux/module.h> /* Needed by all modules */
#include <linux/nmi.h>
#include <linux/printk.h> /* Needed for pr_err() */

#include "device.h"
#include "firmware.h"
#include "main.h"
#include "memory.h"
#include "test/test.h"

static atomic_t call_done;

static void each_cpu_main(void *data) {
    uint cpu = smp_processor_id();
    uint cpus = num_online_cpus();
    unsigned long flag;
    int ret;

    pr_err("disable interrupt on cpu %d.\n", cpu);

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
        pr_err("Leaving Steady Mode.\n");
        atomic_set(&call_done, 0);
    }

    // watchdog_nmi_enable(cpu);

    local_irq_restore(flag);

    pr_err("restore interrupt on cpu %d.\n", cpu);
}

int module_main(void) {
    pr_err("Entering Steady Mode.\n");
    atomic_set(&call_done, 0);
    on_each_cpu(each_cpu_main, NULL, 0);
    return 0;
}

int init_module(void) {
    uint online_cpus;

    pr_err("Hello world from Judge Penguin.\n");
    pr_err("Initializing Judge Penguin.\n");

    online_cpus = num_online_cpus();
    pr_err("online_cpus: %d\n", online_cpus);

    test_rdtsc(TEST_RDTSC_TIMES);

    init_device();
    init_memory();
    load_firmware();

    module_main();

    /* A non 0 return means init_module failed; module can't be loaded. */
    return 0;
}

void cleanup_module(void) {
    pr_err("Cleaning up Judge Penguin.\n");
    free_memory();
    exit_device();
    pr_err("Goodbye world from Judge Penguin.\n");
}

MODULE_LICENSE("GPL");
