#include <stddef.h>

#include "chunk.h"
#include "memory.h"
#include "utils.h"

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
    ft_memcpy(new_allocation, ptr, copy_size);
  }
  free_memory_allocation(ptr, zone);
  return new_allocation;
}
