#ifndef HF_TOKENIZERS_DECODER_FUSE_H
#define HF_TOKENIZERS_DECODER_FUSE_H

#include "_decoders_helper.h"

typedef struct decoder_fuse decoder_fuse;

decoder_fuse *decoder_fuse_new(void);

void decoder_fuse_free(decoder_fuse **decoder);

DEF_DECODER_COMMON(decoder_fuse)

#endif