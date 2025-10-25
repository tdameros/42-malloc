/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 16:45:00 by tdameros          #+#    #+#             */
/*   Updated: 2025/08/07 16:45:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "malloc.h"
#include "printf.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include "allocation.h"
#include "page.h"
#include "chunk.h"

#include <execinfo.h>

void print_backtrace() {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    printf_("Callstack (%zu frames):\n", size);
    for (i = 0; i < size; i++) {
        printf_("  %s\n", strings[i]);
    }
    free(strings);
}
allocations_t malloc_memory = {
    .tiny = NULL,
    .small = NULL,
    .large = NULL,
};



void free_memory_allocation(void *ptr, zone_t **zone) {
    chunk_t *chunk = get_chunk_from_data(ptr);
    page_t *page = chunk->header.page;
    if (get_zone_type(chunk->header.size) == LARGE_ZONE) {
        remove_page(zone, page);
        munmap(page, page->size);
        return;
    }
    remove_allocate_chunk(page, chunk);
    push_front_chunk(&page->free, chunk);
    if (NULL == page->allocation && (NULL != page->previous || NULL != page->next)) {
        remove_page(zone, page);
        munmap(page, page->size);
    }
    return;
}

void free(void *ptr) {
    zone_t **zone = get_zone_from_data(ptr, &malloc_memory);
    free_memory_allocation(ptr, zone);
}


void *get_memory_allocation_medium(size_t aligned_size, zone_t **zone) {
  chunk_t *free_chunk;
  page_t *page = get_page_with_free_chunk(aligned_size, *zone, &free_chunk);
  if (NULL == page) {
    page = allocate_page(aligned_size);
    if (NULL == page) {
      return NULL;
    }
    push_front_page(zone, page);
    free_chunk = page->free;
  }
  remove_free_chunk(page, free_chunk);
  chunk_t *new_free_chunk;
  chunk_t *allocate_chunk = allocate_free_chunk(aligned_size, free_chunk, &new_free_chunk);
  if (NULL != new_free_chunk) {
    push_front_chunk(&page->free, new_free_chunk);
  }
  push_front_chunk(&page->allocation, allocate_chunk);
  return (void *)align_up_power_of_two((size_t) &allocate_chunk->data, ALIGNMENT);
}

void *get_memory_allocation_large(size_t aligned_size, zone_t **zone) {
    page_t *new_page = allocate_page(aligned_size);
    if (NULL == new_page) {
        return NULL;
    }
    new_page->allocation = new_page->free;
    new_page->free = NULL;
    push_front_page(zone, new_page);
    return (void *)align_up_power_of_two((size_t) &new_page->allocation->data, ALIGNMENT);
}

void *get_memory_allocation(size_t size, zone_t **zone) {
    size_t aligned_size = align_up_power_of_two(size, ALIGNMENT);
    if (get_zone_type(aligned_size) == LARGE_ZONE) {
        return get_memory_allocation_large(aligned_size, zone);
    } else {
        return get_memory_allocation_medium(aligned_size, zone);
    }
}

void _putchar(char character) {
    write(1, &character, 1);
}

void *malloc(size_t size) {
    size_t aligned_size = align_up_power_of_two(size, ALIGNMENT);
    zone_t **zone = get_zone(aligned_size, &malloc_memory);
    return get_memory_allocation(aligned_size, zone);
}

void dump_memory(void *ptr, size_t size) {
    if (ptr == NULL) {
        printf("NULL pointer\n");
    } else {
      for (size_t i = 0; i < size; i++) {
        printf("0x%02x ", ((unsigned char *)ptr)[i]);
      }
    }
}

void *realloc_memory_allocation(void *ptr, size_t size, zone_t **zone, zone_t **destination_zone) {
    chunk_t *chunk = get_chunk_from_data(ptr);
    if (chunk->header.size >= size && zone == destination_zone) {
        return ptr;
    }
    void *new_allocation = get_memory_allocation(size, destination_zone);
    if (NULL == new_allocation) {
        return NULL;
    }
    size_t copy_size = chunk->header.size < size ? chunk->header.size : size;
    for (size_t i = 0; i < copy_size; i++) {
        ((uint8_t *)new_allocation)[i] = ((uint8_t *)ptr)[i];
    }
    free_memory_allocation(ptr, zone);
    return new_allocation;
}

void *realloc(void *ptr, size_t size) {
    zone_t **zone = get_zone_from_data(ptr, &malloc_memory);
    zone_t **destination_zone = get_zone(size, &malloc_memory);
    return realloc_memory_allocation(ptr, size, zone, destination_zone);
}
