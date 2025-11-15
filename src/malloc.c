#include "malloc.h"

#include <pthread.h>

#include "allocation.h"
#include "memory.h"
#include "utils.h"

static allocations_t malloc_memory = {
    .tiny = NULL,
    .small = NULL,
    .large = NULL,
};

static pthread_mutex_t malloc_lock = PTHREAD_MUTEX_INITIALIZER;

static void enter_wrapper(void);
static void exit_wrapper(void);

void *malloc(size_t size) {
  enter_wrapper();
  size_t aligned_size = align_up_power_of_two(size, ALIGNMENT);
  zone_t **zone = get_zone_from_size(aligned_size, &malloc_memory);
  void *ptr = allocate_memory_allocation(aligned_size, zone);
  exit_wrapper();
  return ptr;
}

void free(void *ptr) {
  enter_wrapper();
  zone_t **zone = get_zone_from_data(ptr, &malloc_memory);
  free_memory_allocation(ptr, zone);
  exit_wrapper();
}

void *realloc(void *ptr, size_t size) {
  enter_wrapper();
  zone_t **zone = get_zone_from_data(ptr, &malloc_memory);
  zone_t **destination_zone = get_zone_from_size(size, &malloc_memory);
  void *new_ptr = realloc_memory_allocation(ptr, size, zone, destination_zone);
  exit_wrapper();
  return new_ptr;
}

void show_alloc_mem() {
  enter_wrapper();
  print_string("TINY ZONE:\n");
  print_zone(malloc_memory.tiny);
  print_string("SMALL ZONE:\n");
  print_zone(malloc_memory.small);
  print_string("LARGE ZONE:\n");
  print_zone(malloc_memory.large);
  exit_wrapper();
}

static void enter_wrapper(void) {
  pthread_mutex_lock(&malloc_lock);
}

static void exit_wrapper(void) {
  pthread_mutex_unlock(&malloc_lock);
}

void *calloc(size_t nmemb, size_t size) {
  if (size == 0 || SIZE_MAX / size < nmemb) {
    return NULL;
  }
  size_t total_bytes = nmemb * size;
  void *ptr = malloc(total_bytes);
  if (ptr == NULL) {
    return NULL;
  }
  ft_memset(ptr, 0, total_bytes);
  return ptr;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size) {
  if (size == 0 || SIZE_MAX / size < nmemb) {
    return NULL;
  }
  size_t total_bytes = nmemb * size;
  return realloc(ptr, total_bytes);
}
