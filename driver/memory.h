#ifndef JP_MEMORY_H
#define JP_MEMORY_H

#include "page_table.h"

extern virt_addr_t kernel_base;

extern int init_memory(void);

extern void free_memory(void);

#endif