#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void print_char(char c);
void print_string(const char *str);
void print_unumber(size_t number);
void print_unumber_hexa(size_t number);
void print_range_address(const void *address, size_t size);

#endif