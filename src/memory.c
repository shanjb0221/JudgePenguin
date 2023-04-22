#include <linux/io.h>
#include <linux/kallsyms.h>
#include <linux/mm.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/nmi.h>
#include <linux/printk.h> /* Needed for pr_info() */
#include <linux/version.h>
#include <linux/vmalloc.h>

#include "memory.h"

#include "../include/header.h"
#include "test/test.h"

const u64 MEM_START = 0x40000000llu;
const u64 MEM_SIZE = 0x20000000llu; // 512 MiB

// reserve 10 MiB for page table
const u64 PAGE_TABLE_BRK = MEM_START + 0x1000000llu;

static struct resource *mem_res;
static virt_addr_t mem_virt;
virt_addr_t kernel_base;
static typeof(ioremap_page_range) *ioremap_page_range_sym;

void free_memory(void) {
  if (mem_res)
    release_mem_region(mem_res->start, resource_size(mem_res));

  mem_res = NULL;

  if (mem_virt)
    vunmap((void *)mem_virt);
  mem_virt = 0;
}

int init_memory() {
#if defined(CONFIG_KALLSYMS_ALL) && LINUX_VERSION_CODE < KERNEL_VERSION(5, 7, 0)
#define __RESOLVE_EXTERNAL_SYMBOL(symbol)                                      \
  symbol##_sym = (void *)kallsyms_lookup_name(#symbol);                        \
  if (!symbol##_sym)                                                           \
  return -EINVAL
#else
#define __RESOLVE_EXTERNAL_SYMBOL(symbol) symbol##_sym = &symbol
#endif
#define RESOLVE_EXTERNAL_SYMBOL(symbol...) __RESOLVE_EXTERNAL_SYMBOL(symbol)

  RESOLVE_EXTERNAL_SYMBOL(ioremap_page_range);

  // release_mem_region(MEM_START, MEM_SIZE);

  mem_res = request_mem_region(MEM_START, MEM_SIZE, "JudgePenguin MEM");
  if (!mem_res) {
    pr_err("request mem region failed.\n");
    return -1;
  }

  pr_info("request mem region success. start=0x%llx, size=0x%llx", MEM_START,
          MEM_SIZE);

  struct vm_struct *vma;

  vma = get_vm_area(MEM_SIZE, VM_IOREMAP);
  // pr_info("vma: %pK\n", vma);

  if (!vma) {
    pr_err("get vm area failed.\n");
    return -1;
  }
  vma->phys_addr = MEM_START;

  if (ioremap_page_range_sym((unsigned long)vma->addr,
                             (unsigned long)vma->addr + MEM_SIZE,
                             vma->phys_addr, PAGE_KERNEL_EXEC)) {
    pr_err("ioremap page range failed.\n");
    vunmap(vma->addr);
    return -1;
  }
  mem_virt = (u64)vma->addr;

  pr_info("ioremap page range success. virt_addr=0x%pK, size=0x%lx, "
          "phys_addr=0x%llx\n",
          vma->addr, vma->size, vma->phys_addr);

  test_memory(vma->addr, MEM_SIZE);

  pr_info("init memory success.\n");

  u64 page_table_break = init_page_table(MEM_START, mem_virt, MEM_SIZE);
  u64 page_table_size = page_table_break - vma->phys_addr;

  struct file *kernel =
      filp_open("/opt/JudgePenguin/kernel/kernel.bin", O_RDONLY, 0);
  if (IS_ERR(kernel)) {
    pr_err("open kernel failed.\n");
    return -1;
  }

  kernel_base = mem_virt + page_table_size;
  pr_info("kernel base: 0x%llx\n", kernel_base);

  loff_t pos = 0;
  // load kernel to memory
  int ret = kernel_read(kernel, (void *)kernel_base, MEM_SIZE - page_table_size,
                        &pos);

  printk("load %d bytes to kernel\n", ret);

  printk("first 8 bytes: %c%c%c%c%c%c%c%c\n", ((char *)kernel_base)[0],
         ((char *)kernel_base)[1], ((char *)kernel_base)[2],
         ((char *)kernel_base)[3], ((char *)kernel_base)[4],
         ((char *)kernel_base)[5], ((char *)kernel_base)[6],
         ((char *)kernel_base)[7]);

  filp_close(kernel, NULL);

  map_page(MEM_START + page_table_size, kernel_base);

  return 0;
}