#ifndef MALLOC_H
# define MALLOC_H

#include <stddef.h>

#include "allocation.h"

#ifdef TEST_MODE
# define malloc ft_malloc
# define free ft_free
# define realloc ft_realloc
#endif

allocations_t malloc_memory;

void *malloc(size_t size);

#endif