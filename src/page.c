#include <sys/mman.h>
#include <unistd.h>

#include "allocation.h"
#include "page.h"

#include "printf.h"

void push_front_page(zone_t **zone_list, page_t *new_page) {
  if (NULL == *zone_list) {
    *zone_list = new_page;
  } else {
    (*zone_list)->previous = new_page;
    new_page->next = *zone_list;
    *zone_list = new_page;
  }
}

page_t *allocate_page(size_t aligned_size) {
  size_t full_size = get_zone_full_size(aligned_size);
  void* ptr = mmap(NULL, full_size,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS,
                   -1, 0);
  if (MAP_FAILED == ptr) {
    return NULL;
  }
  size_t total_size = align_up_power_of_two(full_size, getpagesize());
  page_t *new_page = ptr;
  chunk_t *new_chunk = ptr + align_up_power_of_two(sizeof(page_t), ALIGNMENT);
  new_chunk->header.size = total_size - align_up_power_of_two(sizeof(page_t), ALIGNMENT) - align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT);
  new_chunk->header.page = new_page;
  new_page->free = new_chunk;
  new_page->size = total_size;
  return new_page;
}

page_t *get_page_with_free_chunk(size_t required_chunk_size, zone_t *zone, chunk_t **free_chunk) {
  page_t *page = zone;
  while (NULL != page) {
    chunk_t *chunk = get_chunk_with_size(required_chunk_size, page->free);
    if (NULL != chunk) {
      if (NULL != free_chunk) {
        *free_chunk = chunk;
      }
      return page;
    }
    page = page->next;
  }
  return NULL;
}

void remove_page(page_t **page, page_t *page_to_remove) {
    if (*page == page_to_remove) {
        *page = page_to_remove->next;
    }
    if (NULL != page_to_remove->previous) {
        page_to_remove->previous->next = page_to_remove->next;
    }
    if (NULL != page_to_remove->next) {
        page_to_remove->next->previous = page_to_remove->previous;
    }
}
