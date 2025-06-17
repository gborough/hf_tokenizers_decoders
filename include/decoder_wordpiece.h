#ifndef HF_TOKENIZERS_DECODER_WORDPIECE_H
#define HF_TOKENIZERS_DECODER_WORDPIECE_H

#include <stdbool.h>

#include "_decoders_helper.h"

typedef struct decoder_wordpiece decoder_wordpiece;

decoder_wordpiece *decoder_wordpiece_new(cstr prefix, bool cleanup);

decoder_wordpiece *decoder_wordpiece_default_new(void);

void decoder_wordpiece_free(decoder_wordpiece **decoder);

DEF_DECODER_COMMON(decoder_wordpiece)

#endif