#include <stdio.h>
#include <assert.h>

#include "decoders.h"
#include "test_decoder_replace.h"

void test_decoder_replace()
{
    decoder_replace *decoder_replace1 = decoder_replace_new(RP_STRING, cstr_from("_"), cstr_from(" "));

    string_vec vec1 = string_vec_init();
    string_vec_push(&vec1, cstr_from("hello"));
    string_vec_push(&vec1, cstr_from("_hello"));
    string_vec decoded1 = decoder_replace_decode_chain(decoder_replace1, vec1);
    cstr res1 = string_vec_to_cstr(decoded1);
    assert(cstr_equals(&res1, "hello hello"));

    decoder_replace *decoder_replace2 = decoder_replace_new(RP_REGEX, cstr_from("ab"), cstr_from(" "));

    string_vec vec2 = string_vec_init();
    string_vec_push(&vec2, cstr_from("aabbb"));
    string_vec decoded2 = decoder_replace_decode_chain(decoder_replace2, vec2);
    cstr res2 = string_vec_to_cstr(decoded2);
    assert(cstr_equals(&res2, "a bb"));

    cstr_drop(&res1);
    cstr_drop(&res2);

    decoder_replace_free(&decoder_replace1);
    decoder_replace_free(&decoder_replace2);
}
