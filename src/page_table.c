#include "page_table.h"
#include <linux/printk.h>
#include <linux/string.h>

const u64 PAGE_SIZE = 1 << 12; // 4 KiB
const u64 PAGE_MASK = ~(PAGE_SIZE - 1);
const u64 PT_ENTRIES = PAGE_SIZE / 8;
const u64 HUGE_PAGE_SIZE = 1 << 21; // 2 MiB
const u64 HUGE_PAGE_MASK = ~(HUGE_PAGE_SIZE - 1);
const u64 P3_PAGE_SIZE = 1 << 30; // 1 GiB
const u64 P3_PAGE_MASK = ~(P3_PAGE_SIZE - 1);

// PTE flags
const u64 PTE_P = 1;           // present
const u64 PTE_W = 1 << 1;      //  writable
const u64 PTE_U = 1 << 2;      // user
const u64 PTE_PWT = 1 << 3;    // page-level write-through
const u64 PTE_PCD = 1 << 4;    // page-level cache disable
const u64 PTE_A = 1 << 5;      // accessed
const u64 PTE_D = 1 << 6;      // dirty
const u64 PTE_H = 1 << 7;      // huge page
const u64 PTE_G = 1 << 8;      // global
const u64 PTE_NE = 1ull << 63; // no-execute

phys_addr_t phys_base_addr;
virt_addr_t virt_base_addr;
u64 total_size;
phys_addr_t next_block_phys_addr;
phys_addr_t P4_base;
phys_addr_t page_table_break;

inline phys_addr_t virt_to_phys(virt_addr_t virt_addr) {
  return virt_addr - virt_base_addr + phys_base_addr;
}
inline virt_addr_t phys_to_virt(phys_addr_t phys_addr) {
  return phys_addr - phys_base_addr + virt_base_addr;
}

inline u64 *PTE(phys_addr_t block_phys_addr, u64 index) {
  return (u64 *)phys_to_virt(block_phys_addr | index * 8);
}

static inline u64 P4_index(virt_addr_t virt_addr) {
  return (virt_addr >> 39) & 0x1ff;
}
static inline u64 P3_index(virt_addr_t virt_addr) {
  return (virt_addr >> 30) & 0x1ff;
}
static inline u64 P2_index(virt_addr_t virt_addr) {
  return (virt_addr >> 21) & 0x1ff;
}
static inline u64 P1_index(virt_addr_t virt_addr) {
  return (virt_addr >> 12) & 0x1ff;
}

u64 div_round_up(u64 x, u64 y) { return (x + y - 1) / y; }
u64 div_round_down(u64 x, u64 y) { return x / y; }
u64 round_up(u64 x, u64 y) { return div_round_up(x, y) * y; }
u64 round_down(u64 x, u64 y) { return div_round_down(x, y) * y; }

inline u64 min(u64 x, u64 y) { return x < y ? x : y; }

void init_page_table_break(void) {
  u64 num_blocks = 5; // reserved
  // P1
  num_blocks += div_round_up(total_size, PAGE_SIZE * PT_ENTRIES);
  // P2
  num_blocks +=
      div_round_up(total_size, PAGE_SIZE * PT_ENTRIES * PT_ENTRIES);
  // P3 & P4
  num_blocks += 3;
  pr_info("page table uses %llu 4K blocks.", num_blocks);

  page_table_break = phys_base_addr + num_blocks * PAGE_SIZE;
  pr_info("page table break set to [p]0x%llx.", page_table_break);
}

phys_addr_t alloc_block(void) {
  phys_addr_t block_phys_addr = next_block_phys_addr;
  next_block_phys_addr += PAGE_SIZE;
  if (next_block_phys_addr > page_table_break) {
    pr_err("page table break!");
    return 0;
  }
  memset((void *)phys_to_virt(block_phys_addr), 0, PAGE_SIZE);
  return block_phys_addr;
}

phys_addr_t init_page_table_4k(void) {
  phys_addr_t P4 = alloc_block();
  pr_info("P4: [p]0x%llx.", P4);
  // trick: recursive mapping P4
  // so that we can access each block by virt_addr [-512 GiB, 0)
  *PTE(P4, PT_ENTRIES - 1) = P4 | PTE_P | PTE_W | PTE_D | PTE_A;

  phys_addr_t P3Upp = alloc_block();
  pr_info("P3[upp]: [p]0x%llx.", P3Upp);
  // physical memory remap to [-1024 GiB, -512 GIB), use P3 Page
  *PTE(P4, PT_ENTRIES - 2) = P3Upp | PTE_P | PTE_W | PTE_D | PTE_A;
  for (u64 i = 0; i < PT_ENTRIES; ++i) {
    *PTE(P3Upp, i) = (i * P3_PAGE_SIZE) | PTE_P | PTE_W | PTE_D | PTE_A | PTE_H;
  }

  phys_addr_t P3Low = alloc_block();
  pr_info("P3[low]: [p]0x%llx.", P3Low);
  // map [phys_base_addr, phys_base_addr + total_size) to [0, total_size)
  *PTE(P4, 0) = P3Low | PTE_P | PTE_W | PTE_D | PTE_A | PTE_U;
  u64 P2Num = div_round_up(total_size, PAGE_SIZE * PT_ENTRIES * PT_ENTRIES);
  for (u64 P3Idx = 0; P3Idx < P2Num; ++P3Idx) {
    u64 P2 = alloc_block();
    pr_info("P2[%llu]: [p]0x%llx.", P3Idx, P2);
    *PTE(P3Low, P3Idx) = P2 | PTE_P | PTE_W | PTE_D | PTE_A | PTE_U;
    u64 P1Num = min(
        div_round_up(total_size - P3Idx * P3_PAGE_SIZE, PAGE_SIZE * PT_ENTRIES),
        PT_ENTRIES);
    for (u64 P2Idx = 0; P2Idx < P1Num; ++P2Idx) {
      u64 P1 = alloc_block();
      *PTE(P2, P2Idx) = P1 | PTE_P | PTE_W | PTE_D | PTE_A;
      u64 P0Num = min(div_round_up(total_size - P3Idx * P3_PAGE_SIZE -
                                       P2Idx * HUGE_PAGE_SIZE,
                                   PAGE_SIZE),
                      PT_ENTRIES);
      for (u64 P1Idx = 0; P1Idx < P0Num; ++P1Idx) {
        u64 P0 =
            P3Idx * P3_PAGE_SIZE + P2Idx * HUGE_PAGE_SIZE + P1Idx * PAGE_SIZE;
        *PTE(P1, P1Idx) = (P0 + phys_base_addr) | PTE_P | PTE_W | PTE_D | PTE_A;
      }
    }
  }

  return P4;
}

phys_addr_t init_page_table(phys_addr_t phys_addr, virt_addr_t virt_addr,
                            u64 size) {
  pr_info("initializing page table...");

  phys_base_addr = phys_addr, virt_base_addr = virt_addr, total_size = size;
  init_page_table_break();
  next_block_phys_addr = phys_addr;

  P4_base = init_page_table_4k();

  pr_info("page table initialized.");
  return page_table_break;
}

void map_page(phys_addr_t phys_addr, virt_addr_t virt_addr) {
  pr_info("mapping [p]0x%llx to [v]0x%llx.", phys_addr, virt_addr);

  u64 P4Idx = P4_index(virt_addr);
  u64 P3Idx = P3_index(virt_addr);
  u64 P2Idx = P2_index(virt_addr);
  u64 P1Idx = P1_index(virt_addr);

  u64 P4 = *PTE(P4_base, P4Idx);
  if (!(P4 & PTE_P)) {
    P4 = alloc_block();
    *PTE(P4_base, P4Idx) = P4 | PTE_P | PTE_W | PTE_D | PTE_A;
  }

  u64 P3 = *PTE(P4, P3Idx);
  if (!(P3 & PTE_P)) {
    P3 = alloc_block();
    *PTE(P4, P3Idx) = P3 | PTE_P | PTE_W | PTE_D | PTE_A;
  }

  u64 P2 = *PTE(P3, P2Idx);
  if (!(P2 & PTE_P)) {
    P2 = alloc_block();
    *PTE(P3, P2Idx) = P2 | PTE_P | PTE_W | PTE_D | PTE_A;
  }

  u64 P1 = *PTE(P2, P1Idx);
  if (!(P1 & PTE_P)) {
    P1 = alloc_block();
    *PTE(P2, P1Idx) = P1 | PTE_P | PTE_W | PTE_D | PTE_A;
  }

  *PTE(P1, P1Idx) = phys_addr | PTE_P | PTE_W | PTE_D | PTE_A;
}