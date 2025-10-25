#include <stddef.h>

#include "allocation.h"
#include "page.h"

chunk_t *get_chunk_with_size(size_t size, chunk_t *chunk) {
  while (NULL != chunk) {
    if (chunk->header.size >= size) {
      return chunk;
    }
    chunk = chunk->header.next;
  }
  return NULL;
}

void remove_free_chunk(page_t *page, chunk_t *chunk_to_free) {
  chunk_t *chunk = page->free;

  if (chunk == chunk_to_free) {
    page->free->header.previous = NULL;
    page->free = chunk_to_free->header.next;
    chunk_to_free->header.previous = NULL;
    chunk_to_free->header.next = NULL;
    return;
  }
  while (NULL != chunk) {
    if (chunk == chunk_to_free) {
      chunk->header.previous->header.next = chunk->header.next;
      chunk->header.next->header.previous = chunk->header.previous;
      chunk_to_free->header.previous = NULL;
      chunk_to_free->header.next = NULL;
      return;
    }
    chunk = chunk->header.next;
  }
}

void remove_allocate_chunk(page_t *page, chunk_t *chunk_to_free) {
  if (page->allocation == chunk_to_free) {
    page->allocation = chunk_to_free->header.next;
  }
  if (NULL != chunk_to_free->header.previous) {
    chunk_to_free->header.previous->header.next = chunk_to_free->header.next;
  }
  if (NULL != chunk_to_free->header.next) {
    chunk_to_free->header.next->header.previous =
        chunk_to_free->header.previous;
  }
}

chunk_t *allocate_free_chunk(size_t size, chunk_t *free_chunk,
                             chunk_t **new_free_chunk) {
  if (free_chunk->header.size - size >
      align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT)) {
    chunk_t *new_free =
        (chunk_t *)((uint8_t *)free_chunk + size +
                    align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT));
    new_free->header.size =
        free_chunk->header.size - size -
        align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT);
    new_free->header.page = free_chunk->header.page;
    *new_free_chunk = new_free;
    free_chunk->header.size = size;
    return free_chunk;
  } else {
    *new_free_chunk = NULL;
    return free_chunk;
  }
}

void push_front_chunk(chunk_t **chunk_list, chunk_t *new_chunk) {
  if (NULL == *chunk_list) {
    *chunk_list = new_chunk;
  } else {
    (*chunk_list)->header.previous = new_chunk;
    new_chunk->header.next = *chunk_list;
    *chunk_list = new_chunk;
  }
}

chunk_t *get_chunk_from_data(void *data) {
  return (chunk_t *)align_down_power_of_two(
      (size_t)data - sizeof(chunk_header_t), ALIGNMENT);
}