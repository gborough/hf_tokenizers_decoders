#ifndef UTILS_UTF8_H
#define UTILS_UTF8_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

bool is_valid_utf8(const uint8_t *data, size_t len);

#endif