
#include <sys/mman.h>

#include "utest/utest.h"

void *ft_malloc(size_t size);

#include "allocation.h"
#include "page.h"
#include "printf.h"
#include "zone.h"
void *allocate_memory_allocation(size_t size, zone_t **zone);
void dump_memory(void *ptr, size_t size);

#define PAGE_HEADER_SIZE align_up_power_of_two(sizeof(page_t), ALIGNMENT)
#define CHUNK_HEADER_SIZE \
  align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT)

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
