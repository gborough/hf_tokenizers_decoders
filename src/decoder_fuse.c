#include <stdlib.h>

#include "decoder_fuse.h"

struct decoder_fuse
{
    cstr type;
};

decoder_fuse *decoder_fuse_new()
{
    decoder_fuse *decoder = malloc(sizeof(decoder_fuse));
    if (!decoder)
        return NULL;

    decoder->type = cstr_from("Fuse");

    return decoder;
}

void decoder_fuse_free(decoder_fuse **decoder)
{
    if (!decoder || !*decoder)
        return;

    decoder_fuse *_decoder = *decoder;
    cstr_drop(&_decoder->type);
    free(_decoder);
    *decoder = NULL;
}

cstr decoder_fuse_decode(const decoder_fuse *decoder, string_vec tokens)
{
    string_vec results = decoder_fuse_decode_chain(decoder, tokens);

    return decode_join(results);
}

string_vec decoder_fuse_decode_chain(__attribute__((unused)) const decoder_fuse *decoder, string_vec tokens)
{
    string_vec result = string_vec_init();
    cstr joined = cstr_init();

    for (c_range(i, string_vec_size(&tokens)))
    {
        cstr_append(&joined, cstr_str(&tokens.data[i]));
    }
    string_vec_push(&result, joined);

    string_vec_drop(&tokens);

    return result;
}