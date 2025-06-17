#ifndef UTILS_STRING_VEC_H
#define UTILS_STRING_VEC_H

#include "stc/cstr.h"

#define T string_vec, cstr
#include "stc/vec.h"

string_vec string_vec_dedup(string_vec sv);
cstr string_vec_to_cstr(string_vec sv);

#endif