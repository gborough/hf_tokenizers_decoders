#include <stdio.h>
#include <assert.h>

#include "decoders.h"
#include "test_decoder_metaspace.h"

void test_decoder_metaspace()
{
    decoder_metaspace *decoder_metaspace1 = decoder_metaspace_default_new();
    string_vec vec1 = string_vec_init();
    string_vec_push(&vec1, cstr_from("▁Hey"));
    string_vec_push(&vec1, cstr_from("▁friend!"));
    string_vec decoded1 = decoder_metaspace_decode_chain(decoder_metaspace1, vec1);
    cstr res1 = string_vec_to_cstr(decoded1);
    assert(cstr_equals(&res1, "Hey friend!"));

    decoder_metaspace *decoder_metaspace2 = decoder_metaspace_new(UNICODE_LOWER_BLOCK, NEVER, true);
    string_vec vec2 = string_vec_init();
    string_vec_push(&vec2, cstr_from("▁Hey"));
    string_vec_push(&vec2, cstr_from("▁friend!"));
    string_vec decoded2 = decoder_metaspace_decode_chain(decoder_metaspace2, vec2);
    cstr res2 = string_vec_to_cstr(decoded2);
    assert(cstr_equals(&res2, " Hey friend!"));

    cstr_drop(&res1);
    cstr_drop(&res2);

    decoder_metaspace_free(&decoder_metaspace1);
    decoder_metaspace_free(&decoder_metaspace2);
}