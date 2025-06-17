#ifndef HF_TOKENIZERS_DECODER_REPLACE_H
#define HF_TOKENIZERS_DECODER_REPLACE_H

#include "_decoders_helper.h"

typedef enum
{
    RP_STRING,
    RP_REGEX
} replace_pattern;

typedef struct decoder_replace decoder_replace;

decoder_replace *decoder_replace_new(replace_pattern rp, cstr search_pat, cstr replace_pat);

void decoder_replace_free(decoder_replace **decoder);

DEF_DECODER_COMMON(decoder_replace)

#endif