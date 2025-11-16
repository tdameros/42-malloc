#ifndef ZONE_H
#define ZONE_H

#include <stddef.h>

#define ALIGNMENT (sizeof(size_t) * 2)
#define TINY_ZONE_SIZE (64u)
#define SMALL_ZONE_SIZE (2048u)
#define NB_ALLOCATION_ZONE (100u)

typedef enum {
    TINY_ZONE,
    SMALL_ZONE,
    LARGE_ZONE,
} zone_type_t;

typedef struct allocations_s allocations_t;
typedef struct page_s page_t;
typedef struct chunk_s chunk_t;

typedef page_t zone_t;

zone_t **get_zone_from_data(void *data, allocations_t *memory);
zone_t **get_zone_from_chunk(const chunk_t *chunk, allocations_t *memory);
zone_t **get_zone_from_type(zone_type_t zone_type, allocations_t *memory);
zone_t **get_zone_from_size(size_t size, allocations_t *memory);
zone_type_t get_zone_type(size_t size);
size_t get_zone_full_size(size_t aligned_size_required);
void print_zone(const zone_t *zone);
#endif
