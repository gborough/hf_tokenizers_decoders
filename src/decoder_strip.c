#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "decoder_strip.h"

struct decoder_strip
{
    char content;
    size_t start;
    size_t stop;
};

decoder_strip *decoder_strip_new(char content, size_t start, size_t stop)
{
    decoder_strip *decoder = malloc(sizeof(decoder_strip));
    if (!decoder)
        return NULL;

    decoder->content = content;
    decoder->start = start;
    decoder->stop = stop;

    return decoder;
}

void decoder_strip_free(decoder_strip **decoder)
{
    if (!decoder || !*decoder)
        return;

    decoder_strip *_decoder = *decoder;
    free(_decoder);
    *decoder = NULL;
}

cstr decoder_strip_decode(const decoder_strip *decoder, string_vec tokens)
{
    string_vec results = decoder_strip_decode_chain(decoder, tokens);

    return decode_join(results);
}

string_vec decoder_strip_decode_chain(const decoder_strip *decoder, string_vec tokens)
{
    string_vec res = string_vec_init();

    for (c_each(s, string_vec, tokens))
    {
        char_vec chars = char_vec_from_cstr(*s.ref);
        isize chars_len = char_vec_size(&chars);

        size_t start_cut = 0;
        for (c_range(i, decoder->start))
        {
            if (chars.data[i] == decoder->content)
            {
                start_cut = i + 1;
                continue;
            }
            else
            {
                break;
            }
        };

        size_t stop_cut = char_vec_size(&chars);
        for (c_range(i, decoder->stop))
        {
            isize index = chars_len - i - 1;
            if (chars.data[index] == decoder->content)
            {
                stop_cut = index;
                continue;
            }
            else
            {
                break;
            }
        };

        cstr collected = char_vec_to_cstr(chars, start_cut, stop_cut);
        string_vec_push(&res, collected);
        char_vec_drop(&chars);
    };

    string_vec_drop(&tokens);

    return res;
}