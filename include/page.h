#ifndef PAGE_H
#define PAGE_H

#include <stddef.h>

#include "zone.h"
#include "chunk.h"

typedef struct page_s {
  chunk_t *allocation;
  chunk_t *free;
  struct page_s *previous;
  struct page_s *next;
  size_t size;
} page_t;

page_t *get_page_with_free_chunk(size_t required_chunk_size, zone_t *zone, chunk_t **free_chunk);
void push_front_page(zone_t **zone_list, page_t *new_page);
void remove_page(page_t **page, page_t *page_to_remove);
page_t *allocate_page(size_t aligned_size);
int32_t free_page(page_t *page);

#endif
