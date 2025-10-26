/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tdameros <tdameros@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 17:34:00 by tdameros          #+#    #+#             */
/*   Updated: 2025/10/26 17:34:00 by tdameros         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
#define TEST_H

#define PAGE_HEADER_SIZE align_up_power_of_two(sizeof(page_t), ALIGNMENT)
#define CHUNK_HEADER_SIZE \
  align_up_power_of_two(sizeof(chunk_header_t), ALIGNMENT)

void ft_free(void *ptr);
void *ft_malloc(size_t size);
void *ft_realloc(void *ptr, size_t size);

#endif