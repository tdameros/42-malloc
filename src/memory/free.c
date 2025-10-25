/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 18:52:00 by tdameros          #+#    #+#             */
/*   Updated: 2025/10/25 18:52:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <sys/mman.h>

#include "chunk.h"
#include "page.h"
#include "zone.h"

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
  if (NULL == page->allocation &&
      (NULL != page->previous || NULL != page->next)) {
    remove_page(zone, page);
    munmap(page, page->size);
  }
  return;
}
