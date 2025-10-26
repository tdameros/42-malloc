#ifndef CHUNK_H
#define CHUNK_H

#include <stddef.h>
#include <stdint.h>

typedef struct page_s page_t;

typedef struct chunk_header_s {
  size_t size;
  struct chunk_s *previous;
  struct chunk_s *next;
  page_t *page;
} chunk_header_t;

typedef struct chunk_s {
  chunk_header_t header;
  uint8_t data;
} chunk_t;

chunk_t *get_chunk_with_size(size_t size, chunk_t *chunk);
chunk_t *get_chunk_from_data(void *data);
void push_front_chunk(chunk_t **chunk_list, chunk_t *new_chunk);
void remove_chunk(chunk_t **chunk_list, chunk_t *chunk_to_remove);
chunk_t *allocate_free_chunk(size_t size, chunk_t *free_chunk, chunk_t **new_free_chunk);

#endif
