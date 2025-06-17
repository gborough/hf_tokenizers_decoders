#include <stdio.h>
#include <assert.h>

#include "decoders.h"
#include "../helpers.h"
#include "test_decoder_fuse.h"

void test_decoder_fuse()
{
    decoder_fuse *decoder_fuse = decoder_fuse_new();

    string_vec vec = string_vec_init();
    string_vec_push(&vec, cstr_from("Hello"));
    string_vec_push(&vec, cstr_from(" friend!"));
    string_vec fused = decoder_fuse_decode_chain(decoder_fuse, vec);
    cstr joined = string_vec_join(fused);
    assert(cstr_equals(&joined, "Hello friend!"));

    cstr_drop(&joined);
    string_vec_drop(&fused);
    decoder_fuse_free(&decoder_fuse);
}