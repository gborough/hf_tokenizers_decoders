#include "decoders.h"
#include "tests.h"

int main()
{
    test_regex();

    test_decoder_byte_fallback();
    test_decoder_ctc();
    test_decoder_fuse();
    test_decoder_metaspace();
    test_decoder_replace();
    test_decoder_strip();
    test_decoder_wordpiece();

    printf("ok\n");

    return 0;
}