#include "helpers.h"

cstr string_vec_join(string_vec token)
{
    cstr result = cstr_from("");

    for (c_range(i, string_vec_size(&token)))
    {
        const char *data = cstr_str(&token.data[i]);
        cstr_append(&result, data);
        if (i + 1 < string_vec_size(&token))
        {
            cstr_append(&result, " ");
        }
    };

    return result;
}
