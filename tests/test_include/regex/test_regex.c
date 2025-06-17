#include <stdio.h>
#include <assert.h>

#include "test_regex.h"
#include "utils/sys_regex.h"

void test_regex()
{
    sys_regex *r = sys_regex_new(cstr_from("\\s+"));

    sys_regex_split_vec sv = sys_regex_find_matches(r, cstr_from("𝔾𝕠𝕠𝕕 𝕞𝕠𝕣𝕟𝕚𝕟𝕘"));

    sys_regex_split f1 = sv.data[0];
    sys_regex_split f2 = sv.data[1];
    sys_regex_split f3 = sv.data[2];

    assert(f1.start == 0);
    assert(f1.end == 16);
    assert(f1.is_match == 0);
    assert(f2.start == 16);
    assert(f2.end == 17);
    assert(f2.is_match == 1);
    assert(f3.start == 17);
    assert(f3.end == 45);
    assert(f3.is_match == 0);

    sys_regex_split_vec_drop(&sv);
    sys_regex_free(&r);
    onig_end();
}