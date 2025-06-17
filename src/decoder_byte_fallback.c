#include <stdlib.h>
#include <ctype.h>

#include "decoder_byte_fallback.h"
#include "utils/utf8.h"

struct decoder_bfb
{
    cstr type;
};

decoder_bfb *decoder_bfb_new()
{
    decoder_bfb *decoder = malloc(sizeof(decoder_bfb));
    if (!decoder)
        return NULL;

    decoder->type = cstr_from("ByteFallback");

    return decoder;
}

void decoder_bfb_free(decoder_bfb **decoder)
{
    if (!decoder || !*decoder)
        return;

    decoder_bfb *_decoder = *decoder;
    cstr_drop(&_decoder->type);
    free(_decoder);
    *decoder = NULL;
}

static bool decode_utf8_bytes(const byte_vec bytes, char **out_str)
{
    size_t len = byte_vec_size(&bytes);
    const uint8_t *raw = bytes.data;

    if (!is_valid_utf8(raw, len))
    {
        *out_str = NULL;
        return false;
    }

    *out_str = malloc(len + 1);
    if (!*out_str)
        return false;

    memcpy(*out_str, raw, len);
    (*out_str)[len] = '\0';
    return true;
}

cstr decoder_bfb_decode(const decoder_bfb *decoder, string_vec tokens)
{
    string_vec results = decoder_bfb_decode_chain(decoder, tokens);

    return decode_join(results);
}

string_vec decoder_bfb_decode_chain(__attribute__((unused)) const decoder_bfb *decoder, string_vec tokens)
{
    string_vec new_tokens = string_vec_init();
    byte_vec previous_byte_tokens = byte_vec_init();

    for (c_range(i, string_vec_size(&tokens)))
    {
        cstr token = tokens.data[i];

        if (cstr_size(&token) == 6 &&
            cstr_starts_with(&token, "<0x") &&
            cstr_ends_with(&token, ">") &&
            isxdigit(cstr_str(&token)[3]) &&
            isxdigit(cstr_str(&token)[4]))
        {
            char hex[3] = {cstr_str(&token)[3], cstr_str(&token)[4], '\0'};
            uint8_t byte = (uint8_t)strtol(hex, NULL, 16);
            byte_vec_push(&previous_byte_tokens, byte);
        }
        else
        {
            if (byte_vec_size(&previous_byte_tokens) > 0)
            {
                char *decoded = NULL;
                if (decode_utf8_bytes(previous_byte_tokens, &decoded))
                {
                    string_vec_push(&new_tokens, cstr_from(decoded));
                    free(decoded);
                }
                else
                {
                    for (c_range(i, byte_vec_size(&previous_byte_tokens)))
                        string_vec_push(&new_tokens, cstr_from("�"));
                }
                byte_vec_drop(&previous_byte_tokens);
                previous_byte_tokens = byte_vec_init();
            }

            string_vec_push(&new_tokens, token);
        }
    };

    if (byte_vec_size(&previous_byte_tokens) > 0)
    {
        char *decoded = NULL;
        if (decode_utf8_bytes(previous_byte_tokens, &decoded))
        {
            string_vec_push(&new_tokens, cstr_from(decoded));
            free(decoded);
        }
        else
        {
            for (c_range(i, byte_vec_size(&previous_byte_tokens)))
                string_vec_push(&new_tokens, cstr_from("�"));
        }
    }

    byte_vec_drop(&previous_byte_tokens);
    string_vec_drop(&tokens);

    return new_tokens;
}