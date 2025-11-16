#include "zone.h"

#include "allocation.h"
#include "page.h"

zone_t **get_zone_from_data(void *data, allocations_t *memory) {
  return get_zone_from_chunk(get_chunk_from_data(data), memory);
}

zone_t **get_zone_from_chunk(const chunk_t *chunk, allocations_t *memory) {
  if (chunk == NULL) {
    return NULL;
  }
  return get_zone_from_type(chunk->header.page->zone_type, memory);
}

zone_t **get_zone_from_type(zone_type_t zone_type, allocations_t *memory) {
  switch (zone_type) {
    case TINY_ZONE:
      return &memory->tiny;
    case SMALL_ZONE:
      return &memory->small;
    case LARGE_ZONE:
      return &memory->large;
  }
  return &memory->large;
}

zone_t **get_zone_from_size(size_t size, allocations_t *memory) {
  if (size <= TINY_ZONE_SIZE) {
    return &memory->tiny;
  } else if (size <= SMALL_ZONE_SIZE) {
    return &memory->small;
  } else {
    return &memory->large;
  }
}

zone_type_t get_zone_type(size_t size) {
  if (size <= TINY_ZONE_SIZE) {
    return TINY_ZONE;
  } else if (size <= SMALL_ZONE_SIZE) {
    return SMALL_ZONE;
  } else {
    return LARGE_ZONE;
  }
}

size_t get_zone_full_size(size_t aligned_size_required) {
  if (aligned_size_required <= TINY_ZONE_SIZE) {
    return align_up_power_of_two(sizeof(page_t), ALIGNMENT) +
           (TINY_ZONE_SIZE +
            align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT)) *
               NB_ALLOCATION_ZONE;
  } else if (aligned_size_required <= SMALL_ZONE_SIZE) {
    return align_up_power_of_two(sizeof(page_t), ALIGNMENT) +
           (SMALL_ZONE_SIZE +
            align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT)) *
               NB_ALLOCATION_ZONE;
  } else {
    return align_up_power_of_two(sizeof(page_t), ALIGNMENT) +
           (aligned_size_required +
            align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT));
  }
}

void print_zone(const zone_t *zone) {
  const page_t *page = find_next_page_in_order(zone, NULL);
  while (NULL != page) {
    print_page(page);
    page = find_next_page_in_order(zone, page);
  }
}
