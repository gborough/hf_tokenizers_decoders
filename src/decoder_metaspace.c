#include <stdlib.h>

#include "stc/cstr.h"
#include "decoder_metaspace.h"

struct decoder_metaspace
{
    utf8proc_int32_t replacement;
    prepend_scheme prepend_scheme;
    bool split;
    cstr str_rep;
};

static cstr cstr_from_char(utf8proc_int32_t ch)
{
    char buf[5] = {0};
    int len = utf8proc_encode_char(ch, (utf8proc_uint8_t *)buf);

    if (len <= 0)
    {
        fprintf(stderr, "Invalid Unicode codepoint: U+%X\n", ch);
        abort();
    }

    cstr s = cstr_init();
    cstr_assign_n(&s, buf, len);

    return s;
}

decoder_metaspace *decoder_metaspace_new(utf8proc_int32_t replacement, prepend_scheme ps, bool split)
{
    decoder_metaspace *decoder = malloc(sizeof(decoder_metaspace));
    if (!decoder)
        return NULL;

    decoder->replacement = replacement;
    decoder->prepend_scheme = ps;
    decoder->split = split;
    decoder->str_rep = cstr_from_char(replacement);

    return decoder;
}

decoder_metaspace *decoder_metaspace_default_new()
{
    decoder_metaspace *decoder = decoder_metaspace_new(UNICODE_LOWER_BLOCK, ALWAYS, true);

    return decoder;
}

void decoder_metaspace_free(decoder_metaspace **decoder)
{
    if (!decoder || !*decoder)
        return;

    decoder_metaspace *_decoder = *decoder;
    cstr_drop(&_decoder->str_rep);
    free(_decoder);
    *decoder = NULL;
}

cstr decoder_metaspace_decode(const decoder_metaspace *decoder, string_vec tokens)
{
    string_vec results = decoder_metaspace_decode_chain(decoder, tokens);

    return decode_join(results);
}

string_vec decoder_metaspace_decode_chain(const decoder_metaspace *decoder, string_vec tokens)
{
    string_vec res = string_vec_init();

    isize outer_loop_index = 0;
    for (c_each(s, string_vec, tokens))
    {
        char_vec chars = char_vec_from_cstr(*s.ref);
        isize chars_len = char_vec_size(&chars);
        char_vec output = char_vec_init();

        for (c_range(i, chars_len))
        {
            utf8proc_int32_t *curr = &chars.data[i];
            if (*curr == decoder->replacement)
            {
                if (outer_loop_index == 0 && decoder->prepend_scheme != NEVER)
                {
                    continue;
                }
                else
                {
                    char_vec_push(&output, ' ');
                }
            }
            else
            {
                char_vec_push(&output, *curr);
            }
        };

        cstr collected = char_vec_to_cstr(output, 0, char_vec_size(&output));
        string_vec_push(&res, collected);
        cstr_drop(&collected);

        char_vec_drop(&output);
        char_vec_drop(&chars);

        ++outer_loop_index;
    };

    string_vec_drop(&tokens);

    return res;
}