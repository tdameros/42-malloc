#include <sys/mman.h>

#include "allocation.h"
#include "memory.h"
#include "page.h"
#include "printf.h"
#include "test.h"
#include "utest/utest.h"
#include "zone.h"

UTEST(allocate_page, zero_size) {
  const page_t *page = allocate_page(0);
  ASSERT_TRUE(page == NULL);
}

UTEST(allocate_page, tiny) {
  page_t *page = allocate_page(TINY_ZONE_SIZE);
  ASSERT_FALSE(page == NULL);
  ASSERT_TRUE(page->allocation == NULL);
  ASSERT_FALSE(page->free == NULL);
  ASSERT_TRUE(page->next == NULL);
  ASSERT_TRUE(page->previous == NULL);
  ASSERT_GE(page->free->header.size,
            (TINY_ZONE_SIZE + CHUNK_HEADER_SIZE) * NB_ALLOCATION_ZONE);
  ASSERT_EQ(munmap(page, page->size), 0);
}

UTEST(allocate_page, small) {
  page_t *page = allocate_page(SMALL_ZONE_SIZE);
  ASSERT_FALSE(page == NULL);
  ASSERT_TRUE(page->allocation == NULL);
  ASSERT_FALSE(page->free == NULL);
  ASSERT_TRUE(page->next == NULL);
  ASSERT_TRUE(page->previous == NULL);
  ASSERT_GE(page->free->header.size,
            (SMALL_ZONE_SIZE + CHUNK_HEADER_SIZE) * NB_ALLOCATION_ZONE);
  ASSERT_EQ(munmap(page, page->size), 0);
}

UTEST(allocate_page, large) {
  page_t *page = allocate_page(SMALL_ZONE_SIZE + 200);
  ASSERT_FALSE(page == NULL);
  ASSERT_TRUE(page->allocation == NULL);
  ASSERT_FALSE(page->free == NULL);
  ASSERT_TRUE(page->next == NULL);
  ASSERT_TRUE(page->previous == NULL);
  ASSERT_GE(page->free->header.size, SMALL_ZONE_SIZE + 200);
  ASSERT_EQ(munmap(page, page->size), 0);
}

UTEST(allocate_memory_allocation, zero_size) {
  zone_t *zone = NULL;
  const void *allocation = allocate_memory_allocation(0, &zone);
  ASSERT_TRUE(allocation == NULL);
  ASSERT_TRUE(zone == NULL);
}

UTEST(allocate_memory_allocation, tiny) {
  size_t malloc_size = TINY_ZONE_SIZE;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  for (uint32_t i = 0; i < NB_ALLOCATION_ZONE; i++) {
    void *allocation = allocate_memory_allocation(aligned_size, &zone);
    ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
    ASSERT_EQ(zone->allocation->header.size, TINY_ZONE_SIZE);
    ASSERT_GE(zone->free->header.size, (TINY_ZONE_SIZE + CHUNK_HEADER_SIZE) *
                                           (NB_ALLOCATION_ZONE - 1 - i));
    ASSERT_EQ(zone->next, NULL);
    ASSERT_EQ(zone->previous, NULL);
  }
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(allocate_memory_allocation, small) {
  size_t malloc_size = SMALL_ZONE_SIZE;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  for (uint32_t i = 0; i < NB_ALLOCATION_ZONE; i++) {
    void *allocation = allocate_memory_allocation(aligned_size, &zone);
    ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
    ASSERT_EQ(zone->allocation->header.size, SMALL_ZONE_SIZE);
    ASSERT_GE(zone->free->header.size, (SMALL_ZONE_SIZE + CHUNK_HEADER_SIZE) *
                                           (NB_ALLOCATION_ZONE - 1 - i));
    ASSERT_EQ(zone->next, NULL);
    ASSERT_EQ(zone->previous, NULL);
  }
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(allocate_memory_allocation, large) {
  size_t malloc_size = SMALL_ZONE_SIZE + 100;
  zone_t *zone = NULL;
  size_t aligned_size = align_up_power_of_two(malloc_size, ALIGNMENT);
  void *allocation = allocate_memory_allocation(aligned_size, &zone);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  ASSERT_GE(zone->allocation->header.size, SMALL_ZONE_SIZE + 100);
  ASSERT_EQ(zone->free, NULL);
  ASSERT_EQ(zone->next, NULL);
  ASSERT_EQ(zone->previous, NULL);
  ASSERT_EQ(munmap(zone, zone->size), 0);
}

UTEST(ft_malloc, zero_size) {
  const void *allocation = ft_malloc(0);
  ASSERT_TRUE(allocation == NULL);
}

UTEST(ft_malloc, tiny) {
  void *allocation = ft_malloc(TINY_ZONE_SIZE);
  ASSERT_FALSE(allocation == NULL);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  memset(allocation, 0xFF, TINY_ZONE_SIZE);
  for (size_t i = 0; i < TINY_ZONE_SIZE; i++) {
    ASSERT_EQ(*((uint8_t *)allocation + i), 0xFF);
  }
  ft_free(allocation);
}

UTEST(ft_malloc, tiny_multiple) {
  void *allocation = ft_malloc(TINY_ZONE_SIZE);
  ASSERT_FALSE(allocation == NULL);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  void *allocation2 = ft_malloc(TINY_ZONE_SIZE);
  ASSERT_FALSE(allocation2 == NULL);
  ASSERT_EQ((size_t)allocation2 % ALIGNMENT, 0u);
  ASSERT_FALSE(allocation == allocation2);
  ft_free(allocation);
  ft_free(allocation2);
}

UTEST(ft_malloc, small) {
  void *allocation = ft_malloc(SMALL_ZONE_SIZE);
  ASSERT_FALSE(allocation == NULL);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  memset(allocation, 0xAA, SMALL_ZONE_SIZE);
  for (size_t i = 0; i < SMALL_ZONE_SIZE; i++) {
    ASSERT_EQ(*((uint8_t *)allocation + i), 0xAA);
  }
  ft_free(allocation);
}

UTEST(ft_malloc, small_multiple) {
  void *allocation = ft_malloc(SMALL_ZONE_SIZE);
  ASSERT_FALSE(allocation == NULL);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  void *allocation2 = ft_malloc(SMALL_ZONE_SIZE);
  ASSERT_FALSE(allocation2 == NULL);
  ASSERT_EQ((size_t)allocation2 % ALIGNMENT, 0u);
  ASSERT_FALSE(allocation == allocation2);
  ft_free(allocation);
  ft_free(allocation2);
}

UTEST(ft_malloc, large) {
  void *allocation = ft_malloc(SMALL_ZONE_SIZE + 500);
  ASSERT_FALSE(allocation == NULL);
  ASSERT_EQ((size_t)allocation % ALIGNMENT, 0u);
  memset(allocation, 0x55, SMALL_ZONE_SIZE + 500);
  for (size_t i = 0; i < SMALL_ZONE_SIZE + 500; i++) {
    ASSERT_EQ(*((uint8_t *)allocation + i), 0x55);
  }
  ft_free(allocation);
}
