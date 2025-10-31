#include <stddef.h>

#include "chunk.h"
#include "memory.h"

void *realloc_memory_allocation(void *ptr, size_t size, zone_t **zone,
                                zone_t **destination_zone) {
  chunk_t *chunk = get_chunk_from_data(ptr);
  if (NULL != chunk && chunk->header.size >= size && zone == destination_zone) {
    return ptr;
  }
  void *new_allocation = allocate_memory_allocation(size, destination_zone);
  if (NULL == new_allocation) {
    return NULL;
  }
  if (NULL != chunk) {
    size_t copy_size = chunk->header.size < size ? chunk->header.size : size;
    for (size_t i = 0; i < copy_size; i++) {
      ((uint8_t *)new_allocation)[i] = ((uint8_t *)ptr)[i];
    }
  }
  free_memory_allocation(ptr, zone);
  return new_allocation;
}
