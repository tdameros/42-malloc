#include <sys/mman.h>

#include "utest/utest.h"

void *ft_free(size_t size);

#include "allocation.h"
#include "page.h"
#include "printf.h"
#include "zone.h"

#define PAGE_HEADER_SIZE align_up_power_of_two(sizeof(page_t), ALIGNMENT)
#define CHUNK_HEADER_SIZE \
  align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT)

void *allocate_memory_allocation(size_t size, zone_t **zone);
void free_memory_allocation(void *ptr, zone_t **zone);
void dump_memory(void *ptr, size_t size);
void *realloc_memory_allocation(void *ptr, size_t size, zone_t **zone,
                                zone_t **destination_zone);

UTEST(realloc_memory_allocation, tiny_simple_downsize) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = TINY_ZONE_SIZE;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &memory.tiny);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  void *reallocation = realloc_memory_allocation(allocation, aligned_size - 10,
                                                 &memory.tiny, &memory.tiny);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_EQ(reallocation, allocation);
  ASSERT_EQ(munmap(memory.tiny, memory.tiny->size), 0);
}

UTEST(realloc_memory_allocation, tiny_simple_upsize) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = TINY_ZONE_SIZE - 3;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &memory.tiny);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  void *reallocation = realloc_memory_allocation(allocation, malloc_size + 3,
                                                 &memory.tiny, &memory.tiny);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_EQ(reallocation, allocation);
  ASSERT_EQ(munmap(memory.tiny, memory.tiny->size), 0);
}

UTEST(realloc_memory_allocation, tiny_upsize_new_pointer) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = TINY_ZONE_SIZE;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  uint8_t *allocation = allocate_memory_allocation(aligned_size, &memory.tiny);
  memset(allocation, 0xA, malloc_size);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  uint8_t *reallocation = realloc_memory_allocation(
      allocation, aligned_size + 10, &memory.tiny, &memory.tiny);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_NE(reallocation, allocation);
  for (size_t i = 0; i < malloc_size; i++) {
    ASSERT_EQ(reallocation[i], 0xA);
  }
  ASSERT_EQ(munmap(memory.tiny, memory.tiny->size), 0);
}

UTEST(realloc_memory_allocation, tiny_to_small) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = TINY_ZONE_SIZE;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  uint8_t *allocation = allocate_memory_allocation(aligned_size, &memory.tiny);
  memset(allocation, 0xA, malloc_size);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  uint8_t *reallocation = realloc_memory_allocation(
      allocation, SMALL_ZONE_SIZE, &memory.tiny, &memory.small);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_NE(reallocation, allocation);
  for (size_t i = 0; i < malloc_size; i++) {
    ASSERT_EQ(reallocation[i], 0xA);
  }
  ASSERT_NE(memory.tiny, NULL);
  ASSERT_EQ(munmap(memory.small, memory.small->size), 0);
}

UTEST(realloc_memory_allocation, small_simple_downsize) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = SMALL_ZONE_SIZE;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &memory.small);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  void *reallocation = realloc_memory_allocation(allocation, aligned_size - 10,
                                                 &memory.small, &memory.small);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_EQ(reallocation, allocation);
  ASSERT_EQ(munmap(memory.small, memory.small->size), 0);
}

UTEST(realloc_memory_allocation, small_simple_upsize) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = SMALL_ZONE_SIZE - 9;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &memory.small);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  void *reallocation = realloc_memory_allocation(allocation, malloc_size + 9,
                                                 &memory.small, &memory.small);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_EQ(reallocation, allocation);
  ASSERT_EQ(munmap(memory.small, memory.small->size), 0);
}

UTEST(realloc_memory_allocation, small_upsize_new_pointer) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = SMALL_ZONE_SIZE;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  uint8_t *allocation = allocate_memory_allocation(aligned_size, &memory.small);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  uint8_t *reallocation = realloc_memory_allocation(
      allocation, malloc_size + 9, &memory.small, &memory.small);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_NE(reallocation, allocation);
  ASSERT_EQ(munmap(memory.small, memory.small->size), 0);
}

UTEST(realloc_memory_allocation, large_simple_downsize) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = SMALL_ZONE_SIZE + 600;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &memory.large);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  void *reallocation = realloc_memory_allocation(allocation, aligned_size - 10,
                                                 &memory.small, &memory.small);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_EQ(reallocation, allocation);
  ASSERT_EQ(munmap(memory.small, memory.large->size), 0);
}

UTEST(realloc_memory_allocation, large_simple_upsize) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = SMALL_ZONE_SIZE + 600;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  uint8_t *allocation = allocate_memory_allocation(aligned_size, &memory.large);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  memset(allocation, 0xA, malloc_size);
  uint8_t *reallocation = realloc_memory_allocation(
      allocation, malloc_size + 10, &memory.large, &memory.large);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_EQ(reallocation, allocation);
  for (size_t i = 0; i < malloc_size; i++) {
    ASSERT_EQ(reallocation[i], 0xA);
  }
  ASSERT_EQ(munmap(memory.large, memory.large->size), 0);
}

UTEST(realloc_memory_allocation, large_upsize_new_pointer) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = SMALL_ZONE_SIZE + 600;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  uint8_t *allocation = allocate_memory_allocation(aligned_size, &memory.large);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  memset(allocation, 0xA, malloc_size);
  ASSERT_NE(memory.large, NULL);
  uint8_t *reallocation = realloc_memory_allocation(
      allocation, malloc_size + 10000, &memory.large, &memory.large);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_NE(reallocation, allocation);
  for (size_t i = 0; i < malloc_size; i++) {
    ASSERT_EQ(reallocation[i], 0xA);
  }
  ASSERT_EQ(munmap(memory.large, memory.large->size), 0);
}

UTEST(realloc_memory_allocation, large_downsize_to_small) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = SMALL_ZONE_SIZE + 600;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &memory.large);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  memset(allocation, 0xA, malloc_size);
  ASSERT_NE(memory.large, NULL);
  void *reallocation = realloc_memory_allocation(allocation, SMALL_ZONE_SIZE,
                                                 &memory.large, &memory.small);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_NE(reallocation, allocation);
  ASSERT_EQ(memory.large, NULL);
  for (size_t i = 0; i < SMALL_ZONE_SIZE; i++) {
    ASSERT_EQ(((uint8_t *)reallocation)[i], 0xA);
  }
  ASSERT_EQ(munmap(memory.small, memory.small->size), 0);
}

UTEST(realloc_memory_allocation, large_downsize_to_tiny) {
  allocations_t memory = {
      .tiny = NULL,
      .small = NULL,
      .large = NULL,
  };
  size_t malloc_size = SMALL_ZONE_SIZE + 600;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &memory.large);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  memset(allocation, 0xA, malloc_size);
  ASSERT_NE(memory.large, NULL);
  void *reallocation = realloc_memory_allocation(allocation, TINY_ZONE_SIZE,
                                                 &memory.large, &memory.tiny);
  ASSERT_EQ((size_t)reallocation % ALIGNMENT, 0u);
  ASSERT_NE(reallocation, allocation);
  ASSERT_EQ(memory.large, NULL);
  for (size_t i = 0; i < TINY_ZONE_SIZE; i++) {
    ASSERT_EQ(((uint8_t *)reallocation)[i], 0xA);
  }
  ASSERT_EQ(munmap(memory.tiny, memory.tiny->size), 0);
}
