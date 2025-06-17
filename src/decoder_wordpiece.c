#include <stdlib.h>

#include "decoder_wordpiece.h"

struct decoder_wordpiece
{
    cstr prefix;
    bool cleanup;
};

decoder_wordpiece *decoder_wordpiece_new(cstr prefix, bool cleanup)
{
    decoder_wordpiece *decoder = malloc(sizeof(decoder_wordpiece));
    if (!decoder)
        return NULL;

    decoder->prefix = prefix;
    decoder->cleanup = cleanup;

    return decoder;
}

decoder_wordpiece *decoder_wordpiece_default_new()
{
    decoder_wordpiece *decoder = malloc(sizeof(decoder_wordpiece));
    if (!decoder)
        return NULL;

    decoder->prefix = cstr_from("##");
    decoder->cleanup = true;

    return decoder;
}

void decoder_wordpiece_free(decoder_wordpiece **decoder)
{
    if (!decoder || !*decoder)
        return;

    decoder_wordpiece *_decoder = *decoder;
    cstr_drop(&_decoder->prefix);
    free(_decoder);
    *decoder = NULL;
}

cstr decoder_wordpiece_decode(const decoder_wordpiece *decoder, string_vec tokens)
{
    string_vec results = decoder_wordpiece_decode_chain(decoder, tokens);

    return decode_join(results);
}

string_vec decoder_wordpiece_decode_chain(const decoder_wordpiece *decoder, string_vec tokens)
{
    string_vec out = string_vec_init();

    for (c_range(i, string_vec_size(&tokens)))
    {
        const cstr token = tokens.data[i];

        cstr result = token;

        if (i != 0)
        {
            size_t prefix_len = cstr_size(&decoder->prefix);

            if (strncmp(cstr_str(&token), cstr_str(&decoder->prefix), prefix_len) == 0)
            {
                const char *rep = "";
                cstr_replace_nfirst(&result, cstr_str(&decoder->prefix), rep, 1);
            }
            else
            {
                cstr tmp = cstr_from(" ");
                cstr_append(&tmp, cstr_str(&result));
                cstr_drop(&result);
                result = tmp;
            }
        }

        if (decoder->cleanup)
        {
            cstr cleaned = clean_up(cstr_str(&result));
            cstr_drop(&result);
            result = cleaned;
        }

        string_vec_push(&out, result);
        cstr_drop(&result);
    };

    string_vec_drop(&tokens);

    return out;
}
