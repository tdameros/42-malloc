/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocation.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 15:37:00 by tdameros          #+#    #+#             */
/*   Updated: 2025/08/08 15:37:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOCATION_H
# define ALLOCATION_H

#include "chunk.h"

#include "zone.h"

inline static size_t align_up_power_of_two(size_t size, size_t alignment) {
    return (size + (alignment - 1)) & ~(alignment - 1);
}

inline static size_t align_down_power_of_two(size_t size, size_t alignment) {
    return size & ~(alignment - 1);
}

typedef struct allocations_s {
    zone_t *tiny;
    zone_t *small;
    zone_t *large;
} allocations_t;

page_t *allocate_page(size_t aligned_size);


#endif