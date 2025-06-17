#include <stdio.h>
#include <assert.h>

#include "decoders.h"
#include "test_decoder_wordpiece.h"

void test_decoder_wordpiece()
{
    decoder_wordpiece *decoder_wordpiece = decoder_wordpiece_new(cstr_from("##"), false);
    string_vec vec = string_vec_init();
    string_vec_push(&vec, cstr_from("##uelo"));
    string_vec_push(&vec, cstr_from("Ara"));
    string_vec_push(&vec, cstr_from("##új"));
    string_vec_push(&vec, cstr_from("##o"));
    string_vec_push(&vec, cstr_from("No"));
    string_vec_push(&vec, cstr_from("##guera"));
    cstr wordpiece = decoder_wordpiece_decode(decoder_wordpiece, vec);
    assert(cstr_equals(&wordpiece, "##uelo Araújo Noguera"));

    cstr_drop(&wordpiece);
    decoder_wordpiece_free(&decoder_wordpiece);
}
