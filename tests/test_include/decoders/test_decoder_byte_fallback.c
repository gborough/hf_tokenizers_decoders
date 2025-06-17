#include <stdio.h>
#include <assert.h>

#include "decoders.h"
#include "../helpers.h"
#include "test_decoder_byte_fallback.h"

void test_decoder_byte_fallback()
{
    decoder_bfb *decoder_bfb = decoder_bfb_new();

    string_vec vec1 = string_vec_init();
    string_vec_push(&vec1, cstr_from("Hello"));
    string_vec_push(&vec1, cstr_from("friend!"));
    string_vec bfb1 = decoder_bfb_decode_chain(decoder_bfb, vec1);
    cstr joined_bfb1 = string_vec_join(bfb1);
    assert(cstr_equals(&joined_bfb1, "Hello friend!"));

    string_vec vec2 = string_vec_init();
    string_vec_push(&vec2, cstr_from("<0x61>"));
    string_vec bfb2 = decoder_bfb_decode_chain(decoder_bfb, vec2);
    cstr joined_bfb2 = string_vec_join(bfb2);
    assert(cstr_equals(&joined_bfb2, "a"));

    string_vec vec3 = string_vec_init();
    string_vec_push(&vec3, cstr_from("<0xE5>"));
    string_vec bfb3 = decoder_bfb_decode_chain(decoder_bfb, vec3);
    cstr joined_bfb3 = string_vec_join(bfb3);
    assert(cstr_equals(&joined_bfb3, "�"));

    string_vec vec4 = string_vec_init();
    string_vec_push(&vec4, cstr_from("<0xE5>"));
    string_vec_push(&vec4, cstr_from("<0x8f>"));
    string_vec bfb4 = decoder_bfb_decode_chain(decoder_bfb, vec4);
    cstr joined_bfb4 = string_vec_join(bfb4);
    assert(cstr_equals(&joined_bfb4, "� �"));

    string_vec vec5 = string_vec_init();
    string_vec_push(&vec5, cstr_from("<0xE5>"));
    string_vec_push(&vec5, cstr_from("<0x8f>"));
    string_vec_push(&vec5, cstr_from("<0xab>"));
    string_vec bfb5 = decoder_bfb_decode_chain(decoder_bfb, vec5);
    cstr joined_bfb5 = string_vec_join(bfb5);
    assert(cstr_equals(&joined_bfb5, "叫"));

    string_vec vec6 = string_vec_init();
    string_vec_push(&vec6, cstr_from("<0xE5>"));
    string_vec_push(&vec6, cstr_from("<0x8f>"));
    string_vec_push(&vec6, cstr_from("<0xab>"));
    string_vec_push(&vec6, cstr_from("a"));
    string_vec bfb6 = decoder_bfb_decode_chain(decoder_bfb, vec6);
    cstr joined_bfb6 = string_vec_join(bfb6);
    assert(cstr_equals(&joined_bfb6, "叫 a"));

    string_vec vec7 = string_vec_init();
    string_vec_push(&vec7, cstr_from("<0xE5>"));
    string_vec_push(&vec7, cstr_from("<0x8f>"));
    string_vec_push(&vec7, cstr_from("a"));
    string_vec bfb7 = decoder_bfb_decode_chain(decoder_bfb, vec7);
    cstr joined_bfb7 = string_vec_join(bfb7);
    assert(cstr_equals(&joined_bfb7, "� � a"));

    cstr_drop(&joined_bfb1);
    cstr_drop(&joined_bfb2);
    cstr_drop(&joined_bfb3);
    cstr_drop(&joined_bfb4);
    cstr_drop(&joined_bfb5);
    cstr_drop(&joined_bfb6);
    cstr_drop(&joined_bfb7);

    string_vec_drop(&bfb1);
    string_vec_drop(&bfb2);
    string_vec_drop(&bfb3);
    string_vec_drop(&bfb4);
    string_vec_drop(&bfb5);
    string_vec_drop(&bfb6);
    string_vec_drop(&bfb7);

    decoder_bfb_free(&decoder_bfb);
}