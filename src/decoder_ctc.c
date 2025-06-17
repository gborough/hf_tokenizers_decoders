#include <stdlib.h>

#include "decoder_ctc.h"

struct decoder_ctc
{
    cstr pad_token;
    cstr word_delimiter_token;
    bool cleanup;
};

decoder_ctc *decoder_ctc_new(const cstr pad_token, const cstr word_delimiter_token, bool cleanup)
{
    decoder_ctc *decoder = malloc(sizeof(decoder_ctc));
    if (!decoder)
        return NULL;

    decoder->pad_token = pad_token;
    decoder->word_delimiter_token = word_delimiter_token;
    decoder->cleanup = cleanup;

    return decoder;
}

decoder_ctc *decoder_ctc_default_new()
{
    const cstr pad_token = cstr_from("<pad>");
    const cstr word_delimiter_token = cstr_from("|");

    decoder_ctc *decoder = malloc(sizeof(decoder_ctc));
    if (!decoder)
        return NULL;

    decoder->pad_token = pad_token;
    decoder->word_delimiter_token = word_delimiter_token;
    decoder->cleanup = true;

    return decoder;
}

void decoder_ctc_free(decoder_ctc **decoder)
{
    if (!decoder || !*decoder)
        return;

    decoder_ctc *_decoder = *decoder;
    cstr_drop(&_decoder->pad_token);
    cstr_drop(&_decoder->word_delimiter_token);
    free(_decoder);
    *decoder = NULL;
}

cstr decoder_ctc_decode(const decoder_ctc *decoder, string_vec tokens)
{
    string_vec results = decoder_ctc_decode_chain(decoder, tokens);

    return decode_join(results);
}

string_vec decoder_ctc_decode_chain(const decoder_ctc *decoder, string_vec tokens)
{
    string_vec res = string_vec_init();
    string_vec deduped = string_vec_dedup(tokens);

    for (c_each(s, string_vec, deduped))
    {
        const cstr *curr = s.ref;
        cstr curr_cloned = cstr_clone(*curr);
        cstr_replace(&curr_cloned, cstr_str(&decoder->pad_token), "");

        if (decoder->cleanup)
        {
            cstr replaced = clean_up(cstr_str(&curr_cloned));
            cstr_drop(&curr_cloned);

            cstr_replace(&replaced, cstr_str(&decoder->word_delimiter_token), " ");
            if (cstr_size(&replaced) > 0)
            {
                string_vec_push(&res, replaced);
            }
            cstr_drop(&replaced);
        }
        else
        {
            if (cstr_size(&curr_cloned) > 0)
                string_vec_push(&res, curr_cloned);

            cstr_drop(&curr_cloned);
        }
    };

    string_vec_drop(&deduped);

    return res;
}
