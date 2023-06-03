#include <linux/io.h>
#include <linux/kallsyms.h>
#include <linux/mm.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/nmi.h>
#include <linux/printk.h> /* Needed for pr_err() */
#include <linux/version.h>
#include <linux/vmalloc.h>

#include "device.h"
#include "memory.h"

#include "test/test.h"

const u64 MEM_START = 0x40000000llu;
const u64 MEM_SIZE = 0x20000000llu;       // 512 MiB
const u64 KERNEL_SIZE = 0x1000000llu;     // 16 MiB
const u64 KERNEL_STACK_SIZE = 0x10000llu; // 64 KiB

static struct resource *mem_res;
static virt_addr_t mem_virt;
virt_addr_t kernel_base, kernel_stack_base, kernel_stack_top;
u64 vp_addr_diff, page_table_size;
static typeof(ioremap_page_range) *ioremap_page_range_sym;

void free_memory(void) {
    pr_err("free memory begin.\n");
    if (mem_res) release_mem_region(mem_res->start, resource_size(mem_res));
    mem_res = NULL;

    if (mem_virt) vunmap((void *)mem_virt);
    mem_virt = 0;
    pr_err("free memory end.\n");
}

int init_memory() {
    pr_err("init memory begin.\n");
    int err;

#if defined(CONFIG_KALLSYMS_ALL) && LINUX_VERSION_CODE < KERNEL_VERSION(5, 7, 0)
#define __RESOLVE_EXTERNAL_SYMBOL(symbol)                                                                              \
    symbol##_sym = (void *)kallsyms_lookup_name(#symbol);                                                              \
    if (!symbol##_sym) return -EINVAL
#else
#define __RESOLVE_EXTERNAL_SYMBOL(symbol) symbol##_sym = &symbol
#endif
#define RESOLVE_EXTERNAL_SYMBOL(symbol...) __RESOLVE_EXTERNAL_SYMBOL(symbol)

    RESOLVE_EXTERNAL_SYMBOL(ioremap_page_range);

    mem_res = request_mem_region(MEM_START, MEM_SIZE, "JudgePenguin MEM");
    if (!mem_res) {
        pr_err("request mem region failed.\n");
        return -1;
    } else {
        pr_err("request mem region success. start=0x%llx, size=0x%llx", MEM_START, MEM_SIZE);
    }

    struct vm_struct *vma = get_vm_area(MEM_SIZE, VM_IOREMAP);
    if (!vma) {
        pr_err("get vm area failed.\n");
        return -1;
    }
    vma->phys_addr = MEM_START;
    mem_virt = (u64)vma->addr;
    vp_addr_diff = mem_virt - MEM_START;

    err = ioremap_page_range_sym(mem_virt, mem_virt + MEM_SIZE, MEM_START, PAGE_KERNEL_EXEC);
    if (err) {
        pr_err("ioremap page range failed.\n");
        vunmap(vma->addr);
        return -1;
    }

    pr_err("ioremap page range success. virt_addr=[v]0x%pK, size=0x%lx, "
           "phys_addr=[p]0x%llx\n",
           vma->addr, vma->size, vma->phys_addr);

    test_memory(vma->addr, MEM_SIZE);

    u64 page_table_break = init_page_table(MEM_START, mem_virt, MEM_SIZE);
    page_table_size = page_table_break - MEM_START;

    kernel_base = mem_virt + page_table_size;
    kernel_stack_base = mem_virt + KERNEL_SIZE - KERNEL_STACK_SIZE;
    kernel_stack_top = mem_virt + KERNEL_SIZE;
    pr_err("kernel base:       [v]0x%llx\n", kernel_base);
    pr_err("kernel stask base: [v]0x%llx\n", kernel_stack_base);
    pr_err("kernel stask top:  [v]0x%llx\n", kernel_stack_top);

    pr_err("init memory end.\n");
    return 0;
}