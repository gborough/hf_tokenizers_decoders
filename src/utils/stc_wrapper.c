#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#define i_implement
#include "utils/byte_vec.h"

#define i_implement
#include "utils/char_vec.h"

#define i_implement
#include "utils/string_vec.h"

#include "stc/cstr.h"

string_vec string_vec_dedup(string_vec sv)
{
    string_vec res = string_vec_init();
    cstr prev = cstr_init();
    bool first_seen = true;

    for (c_each(s, string_vec, sv))
    {
        const cstr *curr = s.ref;

        if (first_seen || !cstr_equals(&prev, cstr_str(curr)))
        {
            string_vec_push(&res, *curr);
            cstr_assign(&prev, cstr_str(curr));
            first_seen = false;
        }
    };

    cstr_drop(&prev);
    string_vec_drop(&sv);

    return res;
}

cstr string_vec_to_cstr(string_vec sv)
{
    cstr res = cstr_init();

    for (c_each(s, string_vec, sv))
    {
        cstr_append(&res, cstr_str(s.ref));
    };

    string_vec_drop(&sv);

    return res;
}

char_vec char_vec_from_cstr(cstr s)
{
    char_vec res = char_vec_init();
    const utf8proc_uint8_t *str = (const utf8proc_uint8_t *)cstr_str(&s);
    ptrdiff_t s_len = (ptrdiff_t)cstr_size(&s);

    ptrdiff_t i = 0;
    while (i < s_len)
    {
        utf8proc_int32_t codepoint;
        ptrdiff_t n = utf8proc_iterate(str + i, s_len - i, &codepoint);
        if (n < 0)
        {
            fprintf(stderr, "Invalid UTF-8 at byte %td\n", i);
            break;
        }
        char_vec_push(&res, codepoint);
        i += n;
    }

    cstr_drop(&s);

    return res;
}

cstr char_vec_to_cstr(const char_vec cv, ptrdiff_t start, ptrdiff_t end)
{
    if (start < 0 || end < 0 || start > cv.size || end > cv.size || start > end)
    {
        fprintf(stderr, "Invalid range: [%td..%td), vec size = %zu\n", start, end, cv.size);
        abort();
    }

    size_t len = (size_t)(end - start);
    size_t max_bytes = len * 4;
    char *buf = malloc(max_bytes + 1);
    if (!buf)
        abort();

    size_t offset = 0;
    for (size_t i = (size_t)start; i < (size_t)end; ++i)
    {
        utf8proc_int32_t cp = cv.data[i];
        int nbytes = utf8proc_encode_char(cp, (utf8proc_uint8_t *)(buf + offset));
        if (nbytes <= 0)
        {
            fprintf(stderr, "Invalid codepoint: U+%X\n", cp);
            free(buf);
            abort();
        }
        offset += (size_t)nbytes;
    }

    buf[offset] = '\0';

    cstr res = cstr_init();
    cstr_assign_n(&res, buf, offset);

    free(buf);
    return res;
}
