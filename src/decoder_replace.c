#include <stdlib.h>
#include <string.h>

#include "decoder_replace.h"
#include "utils/sys_regex.h"
#include "stc/cstr.h"

struct decoder_replace
{
    cstr replace_pat;
    sys_regex *sys_regex;
};

static cstr regex_escape(cstr pat)
{
    const char *meta = "\\.^$|?*+()[]{}";
    cstr escaped = cstr_init();

    for (c_each(ch, cstr, pat))
    {
        if (strchr(meta, *ch.ref))
        {
            const char *dbs = "\\";
            cstr_append(&escaped, dbs);
        }
        else
        {
            const char *pat = ch.ref;
            cstr_append(&escaped, pat);
        }
    }

    cstr_drop(&pat);

    return escaped;
}

decoder_replace *decoder_replace_new(replace_pattern rp, cstr search_pat, cstr replace_pat)
{
    decoder_replace *decoder = malloc(sizeof(decoder_replace));
    if (!decoder)
        return NULL;

    decoder->replace_pat = replace_pat;

    if (rp == RP_STRING)
    {
        sys_regex *sr = sys_regex_new(regex_escape(search_pat));
        if (!sr)
        {
            return NULL;
        }
        decoder->sys_regex = sr;
    }
    else if (rp == RP_REGEX)
    {
        sys_regex *sr = sys_regex_new(search_pat);
        if (!sr)
        {
            return NULL;
        }
        decoder->sys_regex = sr;
    }
    else
    {
        fprintf(stderr, "Invalid Replace Pattern\n");
        return NULL;
    }

    cstr_drop(&replace_pat);
    cstr_drop(&search_pat);

    return decoder;
}

void decoder_replace_free(decoder_replace **decoder)
{
    if (!decoder || !*decoder)
        return;

    decoder_replace *_decoder = *decoder;
    if (_decoder->sys_regex)
        sys_regex_free(&_decoder->sys_regex);

    cstr_drop(&_decoder->replace_pat);
    free(_decoder);
    *decoder = NULL;
}

cstr decoder_replace_decode(const decoder_replace *decoder, string_vec tokens)
{
    string_vec results = decoder_replace_decode_chain(decoder, tokens);

    return decode_join(results);
}

string_vec decoder_replace_decode_chain(const decoder_replace *decoder, string_vec tokens)
{
    string_vec res = string_vec_init();

    for (c_each(s, string_vec, tokens))
    {
        cstr curr_s = *s.ref;
        sys_regex_split_vec srsv = sys_regex_find_matches(decoder->sys_regex, curr_s);
        cstr new_token = cstr_init();

        for (c_each(r, sys_regex_split_vec, srsv))
        {
            sys_regex_split srs = *r.ref;
            if (srs.is_match)
            {
                cstr_append(&new_token, cstr_str(&decoder->replace_pat));
            }
            else
            {
                cstr fragment = cstr_from_s(curr_s, srs.start, srs.end - srs.start);
                cstr_append(&new_token, cstr_str(&fragment));
                cstr_drop(&fragment);
            }
        }

        string_vec_push(&res, new_token);
        sys_regex_split_vec_drop(&srsv);
        cstr_drop(&new_token);
    };

    string_vec_drop(&tokens);
    onig_end();

    return res;
}