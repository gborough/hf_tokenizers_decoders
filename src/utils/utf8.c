#include "utils/utf8.h"
#include "utf8proc.h"

bool is_valid_utf8(const uint8_t *data, size_t len)
{
    utf8proc_ssize_t i = 0;
    utf8proc_int32_t codepoint;

    while (i < (utf8proc_ssize_t)len)
    {
        utf8proc_ssize_t rc = utf8proc_iterate(&data[i], len - i, &codepoint);
        if (rc < 0)
        {
            return false;
        }
        i += rc;
    }
    return true;
}