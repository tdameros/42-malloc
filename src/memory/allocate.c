/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 18:50:00 by tdameros          #+#    #+#             */
/*   Updated: 2025/10/25 18:50:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

#include "allocation.h"
#include "page.h"
#include "zone.h"

static void *allocate_memory_allocation_medium(size_t aligned_size, zone_t **zone);
static void *allocate_memory_allocation_large(size_t aligned_size, zone_t **zone);

void *allocate_memory_allocation(size_t size, zone_t **zone) {
  size_t aligned_size = align_up_power_of_two(size, ALIGNMENT);
  if (get_zone_type(aligned_size) == LARGE_ZONE) {
    return allocate_memory_allocation_large(aligned_size, zone);
  } else {
    return allocate_memory_allocation_medium(aligned_size, zone);
  }
}

static void *allocate_memory_allocation_medium(size_t aligned_size, zone_t **zone) {
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

static void *allocate_memory_allocation_large(size_t aligned_size, zone_t **zone) {
  page_t *new_page = allocate_page(aligned_size);
  if (NULL == new_page) {
    return NULL;
  }
  new_page->allocation = new_page->free;
  new_page->free = NULL;
  push_front_page(zone, new_page);
  return (void *)align_up_power_of_two((size_t) &new_page->allocation->data, ALIGNMENT);
}
