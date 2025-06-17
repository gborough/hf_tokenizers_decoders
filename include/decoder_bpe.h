#ifndef HF_TOKENIZERS_DECODER_BPE_H
#define HF_TOKENIZERS_DECODER_BPE_H

#include "_decoders_helper.h"

typedef struct decoder_bpe decoder_bpe;

decoder_bpe *decoder_bpe_new(const cstr suffix);

decoder_bpe *decoder_bpe_default_new(void);

void decoder_bpe_free(decoder_bpe **decoder);

DEF_DECODER_COMMON(decoder_bpe)

#endif