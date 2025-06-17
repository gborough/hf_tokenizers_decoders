#include <stdio.h>
#include <assert.h>

#include "decoders.h"
#include "test_decoder_ctc.h"

static string_vec id_to_string(cstr s)
{
    string_vec vec = string_vec_init();
    cstr temp = cstr_clone(s);

    char *p = cstr_data(&temp);
    while (*p)
    {
        while (*p == ' ')
            ++p;
        if (*p == '\0')
            break;

        char *word_start = p;
        while (*p && *p != ' ')
            ++p;

        char saved = *p;
        *p = '\0';

        string_vec_push(&vec, cstr_from(word_start));

        *p = saved;
    }

    cstr_drop(&temp);
    cstr_drop(&s);

    return vec;
}

void test_decoder_ctc()
{
    decoder_ctc *decoder_ctc = decoder_ctc_default_new();

    cstr s1 = cstr_from("<pad> <pad> h e e l l <pad> l o o o <pad>");
    string_vec result1 = id_to_string(s1);
    string_vec ctc1 = decoder_ctc_decode_chain(decoder_ctc, result1);
    cstr ctc_res1 = string_vec_to_cstr(ctc1);
    assert(cstr_equals(&ctc_res1, "hello"));

    cstr s2 = cstr_from("<pad> <pad> h e e l l <pad> l o o o <pad> <pad> | <pad> w o o o r <pad> <pad> l l d <pad> <pad> <pad> <pad>");
    string_vec result2 = id_to_string(s2);
    string_vec ctc2 = decoder_ctc_decode_chain(decoder_ctc, result2);
    cstr ctc_res2 = string_vec_to_cstr(ctc2);
    assert(cstr_equals(&ctc_res2, "hello world"));

    cstr s3 = cstr_from("<pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> H <pad> I <pad> S S | | <pad> <pad> <pad> I N <pad> <pad> S <pad> T T <pad> <pad> A N C C T <pad> | | | | | <pad> <pad> <pad> <pad> P <pad> <pad> <pad> <pad> A <pad> <pad> N N N <pad> <pad> I <pad> C <pad> <pad> | | <pad> W <pad> <pad> A S <pad> | | <pad> <pad> <pad> F <pad> <pad> O L <pad> <pad> L L O O W E E D | | <pad> B <pad> <pad> <pad> Y <pad> | | | A | | <pad> S S S <pad> M M <pad> <pad> <pad> A L L <pad> <pad> <pad> <pad> L <pad> | | | <pad> <pad> <pad> <pad> S H H <pad> <pad> <pad> <pad> A R R <pad> <pad> P <pad> <pad> | <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> B <pad> <pad> L L <pad> <pad> <pad> <pad> <pad> O W W <pad> <pad> | | | <pad> <pad> <pad> <pad> <pad> <pad> <pad> H <pad> <pad> <pad> <pad> <pad> <pad> <pad> I G H H | | <pad> <pad> O N <pad> | | H <pad> I S S | | <pad> <pad> C H H <pad> <pad> <pad> E <pad> S S <pad> T T <pad> <pad> | | | <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad> <pad>");
    string_vec result3 = id_to_string(s3);
    string_vec ctc3 = decoder_ctc_decode_chain(decoder_ctc, result3);
    cstr ctc_res3 = string_vec_to_cstr(ctc3);
    assert(cstr_equals(&ctc_res3, "HIS INSTANCT PANIC WAS FOLLOWED BY A SMALL SHARP BLOW HIGH ON HIS CHEST "));

    cstr_drop(&ctc_res1);
    cstr_drop(&ctc_res2);
    cstr_drop(&ctc_res3);

    decoder_ctc_free(&decoder_ctc);
}