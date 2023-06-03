#ifndef JP_PAGING_H
#define JP_PAGING_H

typedef unsigned long long u64;
typedef u64 phys_addr_t;
typedef u64 virt_addr_t;

extern phys_addr_t init_page_table(phys_addr_t phys_addr, virt_addr_t virt_addr, u64 size);

extern void map_page(phys_addr_t phys_addr, virt_addr_t virt_addr);

#endif