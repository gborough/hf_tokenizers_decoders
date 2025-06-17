#ifndef UTILS_CHAR_VEC_H
#define UTILS_CHAR_VEC_H

#include <stddef.h>

#include "utf8proc.h"
#define T char_vec, utf8proc_int32_t
#include "stc/vec.h"

char_vec char_vec_from_cstr(cstr s);
cstr char_vec_to_cstr(const char_vec v, ptrdiff_t start, ptrdiff_t end);

#endif