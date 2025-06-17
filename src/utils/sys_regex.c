#include <string.h>

#define i_implement
#include "utils/sys_regex.h"

#include "utf8proc.h"

sys_regex *sys_regex_new(const cstr replace_pat)
{
    sys_regex *sys_regex = malloc(sizeof(sys_regex));
    if (!sys_regex)
        return NULL;

    OnigErrorInfo err_info;
    const UChar *pattern = (const UChar *)cstr_str(&replace_pat);

    int err = onig_new(
        &sys_regex->compiled,
        pattern,
        pattern + strlen((const char *)pattern),
        ONIG_OPTION_DEFAULT,
        ONIG_ENCODING_UTF8,
        ONIG_SYNTAX_DEFAULT,
        &err_info);

    if (err != ONIG_NORMAL)
    {
        char err_msg[ONIG_MAX_ERROR_MESSAGE_LEN];
        onig_error_code_to_str((UChar *)err_msg, err, &err_info);
        fprintf(stderr, "Oniguruma compile error: %s\n", err_msg);

        free(sys_regex);
        return NULL;
    }

    return sys_regex;
}

void sys_regex_free(sys_regex **sr)
{
    if (!sr || !*sr)
        return;

    sys_regex *_sr = *sr;
    onig_free(_sr->compiled);
    free(_sr);
    *sr = NULL;
}

static size_t advance_utf8_codepoint(const UChar *str, size_t length, size_t offset)
{
    utf8proc_int32_t cp;
    ptrdiff_t iter_len = utf8proc_iterate((const utf8proc_uint8_t *)&str[offset], length - offset, &cp);
    if (iter_len > 0)
        return offset + (size_t)iter_len;

    return offset + 1;
}

matched_indices_vec sys_regex_find_iter(sys_regex *sr, const cstr token)
{
    if (!sr || !sr->compiled)
    {
        fprintf(stderr, "[FATAL] sys_regex_find_iter: NULL sr or sr->compiled\n");
        abort();
    }

    const UChar *subject_str = (const UChar *)cstr_str(&token);
    const size_t subject_length = cstr_size(&token);

    matched_indices_vec miv = matched_indices_vec_init();
    OnigRegex re = sr->compiled;

    OnigRegion *region = onig_region_new();

    size_t offset = 0;

    while (offset <= subject_length)
    {
        int rc = onig_search(
            re,
            subject_str,
            subject_str + subject_length,
            subject_str + offset,
            subject_str + subject_length,
            region,
            ONIG_OPTION_NONE);

        if (rc >= 0)
        {
            size_t start = (size_t)region->beg[0];
            size_t end = (size_t)region->end[0];

            matched_indices r = {start, end};
            matched_indices_vec_push(&miv, r);

            offset = (end > start)
                         ? end
                         : advance_utf8_codepoint(subject_str, subject_length, offset);
        }
        else if (rc == ONIG_MISMATCH)
        {
            size_t next = advance_utf8_codepoint(subject_str, subject_length, offset);
            if (next == offset)
            {
                break;
            }
            offset = next;
        }
        else
        {
            char err_buf[ONIG_MAX_ERROR_MESSAGE_LEN];
            onig_error_code_to_str((UChar *)err_buf, rc);
            fprintf(stderr, "Regex search error: %s\n", err_buf);
            break;
        }
    }

    onig_region_free(region, 1);

    return miv;
}

sys_regex_split_vec sys_regex_find_matches(sys_regex *sr, const cstr token)
{

    if (cstr_is_empty(&token))
    {
        sys_regex_split_vec srpv = sys_regex_split_vec_init();
        sys_regex_split srs = {0, 0, false};
        sys_regex_split_vec_push(&srpv, srs);
        cstr_drop(&token);

        return srpv;
    }
    else
    {
        isize prev = 0;
        isize subject_len = cstr_size(&token);
        sys_regex_split_vec srpv = sys_regex_split_vec_with_capacity(subject_len);
        matched_indices_vec miv = sys_regex_find_iter(sr, token);

        for (c_each(s, matched_indices_vec, miv))
        {
            matched_indices *curr = s.ref;
            if ((size_t)prev != curr->start)
            {
                sys_regex_split srs = {prev, curr->start, false};
                sys_regex_split_vec_push(&srpv, srs);
            }

            sys_regex_split srs = {curr->start, curr->end, true};
            sys_regex_split_vec_push(&srpv, srs);
            prev = curr->end;
        };

        if (prev != subject_len)
        {
            sys_regex_split srs = {prev, subject_len, false};
            sys_regex_split_vec_push(&srpv, srs);
        }

        matched_indices_vec_drop(&miv);
        cstr_drop(&token);

        return srpv;
    }
}