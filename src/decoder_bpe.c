#include <stdlib.h>

#include "decoder_bpe.h"

struct decoder_bpe
{
    cstr suffix;
};

decoder_bpe *decoder_bpe_new(const cstr suffix)
{
    decoder_bpe *decoder = malloc(sizeof(decoder_bpe));
    if (!decoder)
        return NULL;

    decoder->suffix = suffix;

    return decoder;
};

decoder_bpe *decoder_bpe_default_new()
{
    const cstr default_suffix = cstr_from("</w>");

    decoder_bpe *decoder = malloc(sizeof(decoder_bpe));
    if (!decoder)
        return NULL;

    decoder->suffix = default_suffix;

    return decoder;
}

void decoder_bpe_free(decoder_bpe **decoder)
{
    if (!decoder || !*decoder)
        return;

    decoder_bpe *_decoder = *decoder;
    cstr_drop(&_decoder->suffix);
    free(_decoder);
    *decoder = NULL;
}

cstr decoder_bpe_decode(const decoder_bpe *decoder, string_vec tokens)
{
    string_vec results = decoder_bpe_decode_chain(decoder, tokens);

    return decode_join(results);
}

string_vec decoder_bpe_decode_chain(const decoder_bpe *decoder, string_vec tokens)
{
    isize n = string_vec_size(&tokens) - 1;
    string_vec result = string_vec_init();

    for (c_range(i, string_vec_size(&tokens)))
    {
        cstr token = tokens.data[i];
        const char *replacement = (i == n) ? "" : " ";

        cstr t = cstr_clone(token);
        cstr_replace(&t, cstr_str(&decoder->suffix), replacement);
        string_vec_push(&result, t);
    }

    string_vec_drop(&tokens);
    return result;
}
