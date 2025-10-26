#include <sys/mman.h>

#include "allocation.h"
#include "memory.h"
#include "page.h"
#include "printf.h"
#include "test.h"
#include "utest/utest.h"
#include "zone.h"

UTEST(free_memory_allocation, tiny_simple) {
  size_t malloc_size = TINY_ZONE_SIZE;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &zone);
  ASSERT_EQ(zone->allocation->header.size, TINY_ZONE_SIZE);
  ASSERT_GE(zone->free->header.size,
            (TINY_ZONE_SIZE + CHUNK_HEADER_SIZE) * (NB_ALLOCATION_ZONE - 1));
  ASSERT_EQ(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  free_memory_allocation(allocation, &zone);
  ASSERT_EQ(zone->allocation, NULL);
  // Change when defrag implemented
  ASSERT_GE(zone->free->header.size, aligned_size);
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(free_memory_allocation, tiny_multiple) {
  size_t malloc_size = TINY_ZONE_SIZE;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &zone);
  allocate_memory_allocation(aligned_size, &zone);
  allocate_memory_allocation(aligned_size, &zone);
  ASSERT_EQ(zone->allocation->header.size, TINY_ZONE_SIZE);
  ASSERT_GE(zone->free->header.size,
            (TINY_ZONE_SIZE + CHUNK_HEADER_SIZE) * (NB_ALLOCATION_ZONE - 3));
  ASSERT_EQ(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  free_memory_allocation(allocation, &zone);
  ASSERT_EQ(zone->allocation->header.size, aligned_size);
  // Change when defrag implemented
  ASSERT_GE(zone->free->header.size, aligned_size);
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(free_memory_allocation, tiny_multiple_pages) {
  size_t malloc_size = TINY_ZONE_SIZE;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocations[NB_ALLOCATION_ZONE * 10];
  for (uint32_t i = 0; i < NB_ALLOCATION_ZONE * 10; i++) {
    allocations[i] = allocate_memory_allocation(aligned_size, &zone);
    ASSERT_EQ(zone->allocation->header.size, TINY_ZONE_SIZE);
  }
  ASSERT_NE(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  for (uint32_t i = 0; i < NB_ALLOCATION_ZONE * 10; i++) {
    free_memory_allocation(allocations[i], &zone);
  }
  ASSERT_EQ(zone->allocation, NULL);
  ASSERT_EQ(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(free_memory_allocation, small_simple) {
  size_t malloc_size = SMALL_ZONE_SIZE;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &zone);
  ASSERT_EQ(zone->allocation->header.size, SMALL_ZONE_SIZE);
  ASSERT_GE(zone->free->header.size,
            (SMALL_ZONE_SIZE + CHUNK_HEADER_SIZE) * (NB_ALLOCATION_ZONE - 1));
  ASSERT_EQ(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  free_memory_allocation(allocation, &zone);
  ASSERT_EQ(zone->allocation, NULL);
  // Change when defrag implemented
  ASSERT_GE(zone->free->header.size, aligned_size);
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(free_memory_allocation, small_multiple) {
  size_t malloc_size = SMALL_ZONE_SIZE;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &zone);
  allocate_memory_allocation(aligned_size, &zone);
  allocate_memory_allocation(aligned_size, &zone);
  ASSERT_EQ(zone->allocation->header.size, SMALL_ZONE_SIZE);
  ASSERT_GE(zone->free->header.size,
            (SMALL_ZONE_SIZE + CHUNK_HEADER_SIZE) * (NB_ALLOCATION_ZONE - 3));
  ASSERT_EQ(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  free_memory_allocation(allocation, &zone);
  ASSERT_EQ(zone->allocation->header.size, aligned_size);
  // Change when defrag implemented
  ASSERT_GE(zone->free->header.size, aligned_size);
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(free_memory_allocation, small_multiple_pages) {
  size_t malloc_size = SMALL_ZONE_SIZE;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocations[NB_ALLOCATION_ZONE * 10];
  for (uint32_t i = 0; i < NB_ALLOCATION_ZONE * 10; i++) {
    allocations[i] = allocate_memory_allocation(aligned_size, &zone);
    ASSERT_EQ(zone->allocation->header.size, SMALL_ZONE_SIZE);
  }
  ASSERT_NE(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  for (uint32_t i = 0; i < NB_ALLOCATION_ZONE * 10; i++) {
    free_memory_allocation(allocations[i], &zone);
  }
  ASSERT_EQ(zone->allocation, NULL);
  ASSERT_EQ(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(free_memory_allocation, medium_simple) {
  size_t malloc_size = SMALL_ZONE_SIZE + 150;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &zone);
  ASSERT_GE(zone->allocation->header.size, malloc_size);
  ASSERT_EQ(zone->free, NULL);
  ASSERT_EQ(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  free_memory_allocation(allocation, &zone);
  ASSERT_EQ(zone, NULL);
}

UTEST(free_memory_allocation, medium_multiple_pages) {
  size_t malloc_size = SMALL_ZONE_SIZE + 42;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &zone);
  void *allocation2 = allocate_memory_allocation(aligned_size, &zone);
  void *allocation3 = allocate_memory_allocation(aligned_size, &zone);
  ASSERT_GE(zone->allocation->header.size, aligned_size);
  ASSERT_EQ(zone->free, NULL);
  ASSERT_EQ(zone->previous, NULL);
  ASSERT_NE(zone->next, NULL);
  ASSERT_NE(zone->next->next, NULL);
  free_memory_allocation(allocation, &zone);
  ASSERT_GE(zone->allocation->header.size, aligned_size);
  free_memory_allocation(allocation2, &zone);
  ASSERT_GE(zone->allocation->header.size, aligned_size);
  free_memory_allocation(allocation3, &zone);
  ASSERT_EQ(zone, NULL);
}

UTEST(ft_free, null_pointer) {
  ft_free(NULL);
  ASSERT_EQ(1, 1);
}

UTEST(ft_free, tiny_allocation) {
  void *allocation = ft_malloc(TINY_ZONE_SIZE);
  ASSERT_NE(allocation, NULL);
  ft_free(allocation);
}
UTEST(ft_free, small_allocation) {
  void *allocation = ft_malloc(SMALL_ZONE_SIZE);
  ASSERT_NE(allocation, NULL);
  ft_free(allocation);
}
UTEST(ft_free, medium_allocation) {
  void *allocation = ft_malloc(SMALL_ZONE_SIZE + 500);
  ASSERT_NE(allocation, NULL);
  ft_free(allocation);
}
