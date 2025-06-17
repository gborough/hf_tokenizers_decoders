#ifndef UTILS_SYS_REGEX_H
#define UTILS_SYS_REGEX_H

#include <stddef.h>
#include <stdbool.h>

#include "oniguruma.h"

typedef struct sys_regex
{
    OnigRegex compiled;
} sys_regex;

typedef struct matched_indices
{
    size_t start, end;
} matched_indices;

typedef struct sys_regex_split
{
    size_t start, end;
    bool is_match;
} sys_regex_split;

#define T matched_indices_vec, matched_indices
#include "stc/vec.h"

#define T sys_regex_split_vec, sys_regex_split
#include "stc/vec.h"

#include "stc/cstr.h"

sys_regex *sys_regex_new(const cstr pattern_str);

void sys_regex_free(sys_regex **sr);

matched_indices_vec sys_regex_find_iter(sys_regex *sr, const cstr subject);

sys_regex_split_vec sys_regex_find_matches(sys_regex *sr, const cstr subject);

#endif