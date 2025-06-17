#ifndef HF_TOKENIZERS_DECODER_STRIP_H
#define HF_TOKENIZERS_DECODER_STRIP_H

#include <stddef.h>

#include "_decoders_helper.h"

typedef struct decoder_strip decoder_strip;

decoder_strip *decoder_strip_new(char content, size_t start, size_t stop);

void decoder_strip_free(decoder_strip **decoder);

DEF_DECODER_COMMON(decoder_strip)

#endif