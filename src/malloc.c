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

#include "allocation.h"
#include "memory.h"

allocations_t malloc_memory = {
    .tiny = NULL,
    .small = NULL,
    .large = NULL,
};

void *malloc(size_t size) {
  size_t aligned_size = align_up_power_of_two(size, ALIGNMENT);
  zone_t **zone = get_zone(aligned_size, &malloc_memory);
  return allocate_memory_allocation(aligned_size, zone);
}

void free(void *ptr) {
  zone_t **zone = get_zone_from_data(ptr, &malloc_memory);
  free_memory_allocation(ptr, zone);
}

void *realloc(void *ptr, size_t size) {
  zone_t **zone = get_zone_from_data(ptr, &malloc_memory);
  zone_t **destination_zone = get_zone(size, &malloc_memory);
  return realloc_memory_allocation(ptr, size, zone, destination_zone);
}
