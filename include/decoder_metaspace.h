#ifndef HF_TOKENIZERS_DECODER_METASPACE_H
#define HF_TOKENIZERS_DECODER_METASPACE_H

#include <stdbool.h>

#include "_decoders_helper.h"
#include "utf8proc.h"

#define UNICODE_LOWER_BLOCK 0x2581

typedef enum
{
    FIRST,
    NEVER,
    ALWAYS
} prepend_scheme;

typedef struct decoder_metaspace decoder_metaspace;

decoder_metaspace *decoder_metaspace_new(utf8proc_int32_t replacement, prepend_scheme ps, bool split);

decoder_metaspace *decoder_metaspace_default_new(void);

void decoder_metaspace_free(decoder_metaspace **decoder);

DEF_DECODER_COMMON(decoder_metaspace)

#endif