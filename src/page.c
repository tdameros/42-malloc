#include "page.h"

#include <sys/mman.h>
#include <unistd.h>

#include "allocation.h"
#include "chunk.h"
#include "utils.h"

page_t *get_page_with_free_chunk(size_t required_chunk_size, zone_t *zone,
                                 chunk_t **free_chunk) {
  page_t *page = zone;
  print_string("search a page..\n");
  while (NULL != page) {
    print_string("searching in page: ");
    print_unumber_hexa((size_t) page);
    print_string("\n");
    print_unumber((size_t)page->size);
    print_string("Boom\n");
    chunk_t *chunk = get_chunk_with_size(required_chunk_size, page->free);
    print_string("chunk founded!\n");
    if (NULL != chunk) {
      if (NULL != free_chunk) {
        *free_chunk = chunk;
      }
      return page;
    }
    print_string("s\n");
    page = page->next;
  }
  return NULL;
}

void push_front_page(zone_t **zone_list, page_t *new_page) {
  if (NULL == *zone_list) {
    *zone_list = new_page;
  } else {
    (*zone_list)->previous = new_page;
    new_page->next = *zone_list;
    *zone_list = new_page;
  }
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
  page_to_remove->next = NULL;
  page_to_remove->previous = NULL;
}

page_t *allocate_page(size_t aligned_size) {
  if (0 == aligned_size) {
    return NULL;
  }
  size_t full_size = get_zone_full_size(aligned_size);
  void *ptr = mmap(NULL, full_size, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (MAP_FAILED == ptr) {
    return NULL;
  }
  size_t total_size = align_up_power_of_two(full_size, getpagesize());
  page_t *new_page = ptr;
  chunk_t *new_chunk = ptr + align_up_power_of_two(sizeof(page_t), ALIGNMENT);
  new_chunk->header.size =
      total_size - align_up_power_of_two(sizeof(page_t), ALIGNMENT) -
      align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT);
  new_chunk->header.page = new_page;
  new_page->free = new_chunk;
  new_page->size = total_size;
  new_page->zone_type = get_zone_type(aligned_size);
  return new_page;
}

#include "utils.h"
int32_t free_page(page_t *page) {
  int32_t i = munmap(page, page->size);
  if (i != 0) {
    print_string("FREE PAGE FAILED\n");
  }
  print_string("Page freed: ");
  print_unumber_hexa((size_t) page);
  print_string("\n");
  return i;
}

const page_t *find_next_page_in_order(const page_t *page_list,
                                      const page_t *after) {
  const page_t *current = page_list;
  const page_t *next = NULL;

  while (current != NULL) {
    if ((after == NULL || current > after) &&
        (next == NULL || current < next)) {
      next = current;
    }
    current = current->next;
  }
  return next;
}

void print_page(const page_t *page) {
  print_range_address(page, page->size);
  print_string("  Allocation chunks:\n");
  const chunk_t *chunk = find_next_chunk_in_order(page->allocation, NULL);
  if (NULL == chunk) {
    print_string("    None\n");
  }
  while (NULL != chunk) {
    print_string("    ");
    print_chunk(chunk);
    chunk = find_next_chunk_in_order(page->allocation, chunk);
  }
  print_string("  Free chunks:\n");
  chunk = find_next_chunk_in_order(page->free, NULL);
  if (NULL == chunk) {
    print_string("    None\n");
  }
  while (NULL != chunk) {
    print_string("    ");
    print_chunk(chunk);
    chunk = find_next_chunk_in_order(page->free, chunk);
  }
}