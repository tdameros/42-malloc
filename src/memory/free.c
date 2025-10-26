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

#include "chunk.h"
#include "page.h"
#include "zone.h"

int32_t free_memory_allocation(void *ptr, zone_t **zone) {
  if (NULL == ptr) {
    return -1;
  }
  chunk_t *chunk = get_chunk_from_data(ptr);
  page_t *page = chunk->header.page;
  if (get_zone_type(chunk->header.size) == LARGE_ZONE) {
    remove_page(zone, page);
    return free_page(page);
  }
  remove_chunk(&page->allocation, chunk);
  push_front_chunk(&page->free, chunk);
  if (NULL == page->allocation &&
      (NULL != page->previous || NULL != page->next)) {
    remove_page(zone, page);
    return free_page(page);
  }
  return 0;
}
