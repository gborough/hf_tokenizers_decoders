#ifndef HF_TOKENIZERS_DECODERS_HELPER_H
#define HF_TOKENIZERS_DECODERS_HELPER_H

#include "utils/byte_vec.h"
#include "utils/char_vec.h"
#include "utils/string_vec.h"

#define DEF_DECODER_COMMON(type)                                \
    cstr type##_decode(const type *decoder, string_vec tokens); \
    string_vec type##_decode_chain(const type *decoder, string_vec tokens);

static inline cstr decode_join(string_vec decoded)
{
    cstr joined = cstr_from("");

    for (c_range(i, string_vec_size(&decoded)))
        cstr_append(&joined, cstr_str(&decoded.data[i]));

    string_vec_drop(&decoded);

    return joined;
}

static inline cstr clean_up(const char *input)
{
    typedef struct
    {
        const char *from;
        const char *to;
    } replacement;

    static const replacement replacements[] = {
        {" .", "."},
        {" ?", "?"},
        {" !", "!"},
        {" ,", ","},
        {" ' ", "'"},
        {" n't", "n't"},
        {" 'm", "'m"},
        {" do not", " don't"},
        {" 's", "'s"},
        {" 've", "'ve"},
        {" 're", "'re"}};

    cstr s = cstr_from(input);

    for (size_t i = 0; i < sizeof(replacements) / sizeof(replacements[0]); ++i)
    {
        cstr_replace(&s, replacements[i].from, replacements[i].to);
    }

    return s;
}

#endif