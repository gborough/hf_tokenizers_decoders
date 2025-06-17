#include <stdio.h>
#include <assert.h>

#include "decoders.h"
#include "test_decoder_strip.h"

void test_decoder_strip()
{
    decoder_strip *decoder_strip1 = decoder_strip_new('H', 1, 0);

    string_vec vec1 = string_vec_init();
    string_vec_push(&vec1, cstr_from("Hey"));
    string_vec_push(&vec1, cstr_from(" friend!"));
    string_vec_push(&vec1, cstr_from("HHH"));
    string_vec decoded1 = decoder_strip_decode_chain(decoder_strip1, vec1);
    cstr res1 = string_vec_to_cstr(decoded1);
    assert(cstr_equals(&res1, "ey friend!HH"));

    decoder_strip *decoder_strip2 = decoder_strip_new('y', 0, 1);
    string_vec vec2 = string_vec_init();
    string_vec_push(&vec2, cstr_from("Hey"));
    string_vec_push(&vec2, cstr_from(" friend!"));
    string_vec decoded2 = decoder_strip_decode_chain(decoder_strip2, vec2);
    cstr res2 = string_vec_to_cstr(decoded2);
    assert(cstr_equals(&res2, "He friend!"));

    cstr_drop(&res1);
    cstr_drop(&res2);

    decoder_strip_free(&decoder_strip1);
    decoder_strip_free(&decoder_strip2);
}