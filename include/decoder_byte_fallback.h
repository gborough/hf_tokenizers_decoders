#ifndef HF_TOKENIZERS_DECODER_BYTE_FALLBACK_H
#define HF_TOKENIZERS_DECODER_BYTE_FALLBACK_H

#include "_decoders_helper.h"

typedef struct decoder_bfb decoder_bfb;

decoder_bfb *decoder_bfb_new(void);

void decoder_bfb_free(decoder_bfb **decoder);

DEF_DECODER_COMMON(decoder_bfb)

#endif