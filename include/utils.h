#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void print_char(char c);
void print_string(const char *str);
void print_unumber(size_t number);
void print_unumber_hexa(size_t number);
void print_range_address(const void *address, size_t size);
void ft_memset(void *ptr, int value, size_t num);
void ft_memcpy(void *dest, const void *src, size_t n);

#endif