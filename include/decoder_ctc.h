#ifndef HF_TOKENIZERS_DECODER_CTC_H
#define HF_TOKENIZERS_DECODER_CTC_H

#include <stdbool.h>
#include "_decoders_helper.h"

typedef struct decoder_ctc decoder_ctc;

decoder_ctc *decoder_ctc_new(const cstr pad_token, const cstr word_delimiter_token, bool cleanup);

decoder_ctc *decoder_ctc_default_new(void);

void decoder_ctc_free(decoder_ctc **decoder);

DEF_DECODER_COMMON(decoder_ctc)

#endif