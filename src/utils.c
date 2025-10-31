#include <stdint.h>
#include <unistd.h>

void print_char(char c) {
  write(1, &c, 1);
}

void print_string(const char *str) {
  size_t i = 0;
  while (str[i] != '\0') {
    i++;
  }
  write(1, str, i);
}

void print_unumber(size_t number) {
  if (number >= 10) {
    print_unumber(number / 10);
  }
  char digit = '0' + (number % 10);
  print_char(digit);
}

void print_unumber_hexa(size_t number) {
  const char *hex_chars = "0123456789ABCDEF";
  if (number >= 16) {
    print_unumber_hexa(number / 16);
  }
  char digit = hex_chars[number % 16];
  print_char(digit);
}

void print_range_address(const void *address, size_t size) {
  print_unumber_hexa((size_t)address);
  print_string(" - ");
  print_unumber_hexa((size_t)address + size);
  print_string(": ");
  print_unumber(size);
  print_string(" bytes\n");
}

void ft_memset(void *ptr, int value, size_t num) {
  uint8_t *byte_ptr = ptr;
  for (size_t i = 0; i < num; i++) {
    byte_ptr[i] = value;
  }
}

void ft_memcpy(void *dest, const void *src, size_t n) {
  uint8_t *byte_dest = dest;
  const uint8_t *byte_src = src;
  for (size_t i = 0; i < n; i++) {
    byte_dest[i] = byte_src[i];
  }
}