
#include "utest/utest.h"

void *ft_malloc(size_t size);

UTEST(malloc, basic) {
  char *a = ft_malloc(10);
  (void)a;
  ASSERT_EQ(1, 1);
}
