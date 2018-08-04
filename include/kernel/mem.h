#include <kernel/atag.h>
#include <kernel/list.h>

#ifndef MEM_H
#define MEM_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define KERNEL_HEAP_SIZE (1024*1024)

typedef struct {
  uint8_t allocated: 1;
  uint8_t kernel_page: 1;
  uint8_t kernel_heap_page: 1;
  uint32_t reserved: 29;
} page_flags_t;

typedef struct page {
  uint32_t vaddr_mapped;
  page_flags_t flags;
  DEFINE_LINK(page);
} page_t;

void mem_init(atag_t * tags);

void * alloc_page(void);
void free_page(void * ptr);

void * kmalloc(uint32_t bytes);
void kfree(void * ptr);

#endif
