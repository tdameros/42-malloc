#include <stddef.h>

#include "allocation.h"
#include "page.h"
#include "utils.h"

chunk_t *get_chunk_with_size(size_t size, chunk_t *chunk) {
  print_string("search a chunk\n");
  while (NULL != chunk) {
    if (chunk->header.size >= size) {
      return chunk;
    }
    chunk = chunk->header.next;
  }
  return NULL;
}

chunk_t *get_chunk_from_data(void *data) {
  if (NULL == data) {
    return NULL;
  }
  return (chunk_t *)align_down_power_of_two(
      (size_t)data - sizeof(chunk_header_t), ALIGNMENT);
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

void remove_chunk(chunk_t **chunk_list, chunk_t *chunk_to_remove) {
  if (*chunk_list == chunk_to_remove) {
    *chunk_list = chunk_to_remove->header.next;
  }
  if (NULL != chunk_to_remove->header.previous) {
    chunk_to_remove->header.previous->header.next =
        chunk_to_remove->header.next;
  }
  if (NULL != chunk_to_remove->header.next) {
    chunk_to_remove->header.next->header.previous =
        chunk_to_remove->header.previous;
  }
  chunk_to_remove->header.next = NULL;
  chunk_to_remove->header.previous = NULL;
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
    new_free->header.previous = NULL;
    new_free->header.next = NULL;
    new_free->header.page = free_chunk->header.page;
    *new_free_chunk = new_free;
    free_chunk->header.size = size;
    return free_chunk;
  } else {
    *new_free_chunk = NULL;
    return free_chunk;
  }
}

chunk_t *find_next_chunk_in_order(chunk_t *chunk_list, const chunk_t *after) {
  chunk_t *current = chunk_list;
  chunk_t *next = NULL;

  while (current != NULL) {
    if ((after == NULL || current > after) &&
        (next == NULL || current < next)) {
      next = current;
    }
    current = current->header.next;
  }

  return next;
}

void print_chunk(const chunk_t *chunk) {
  const void *chunk_data =
      (void *)align_up_power_of_two((size_t)&chunk->data, ALIGNMENT);
  print_range_address(chunk_data, chunk->header.size);
}
