#ifndef JP_MEMORY_H
#define JP_MEMORY_H

#include "page_table.h"

extern const u64 MEM_START;
extern const u64 MEM_SIZE;
extern const u64 KERNEL_SIZE; // kernel size limit
extern const u64 KERNEL_STACK_SIZE;

extern virt_addr_t kernel_base;
extern u64 vp_addr_diff;
extern u64 page_table_size;
extern virt_addr_t kernel_stack_base;
extern virt_addr_t kernel_stack_top;

extern int init_memory(void);

extern void free_memory(void);

#endif