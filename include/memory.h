/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/25 18:51:00 by tdameros          #+#    #+#             */
/*   Updated: 2025/10/25 18:51:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

#include "zone.h"

void free_memory_allocation(void *ptr, zone_t **zone);
void *allocate_memory_allocation(size_t size, zone_t **zone);
void *realloc_memory_allocation(void *ptr, size_t size, zone_t **zone, zone_t **destination_zone);

#endif