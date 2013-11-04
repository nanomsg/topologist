#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

void inf_report_error(struct inf_context *ctx, const char *fmt, ...)
{
    (void) ctx;  /* Build context */
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

struct cfg_m_str_m_str_pair_options *inf_get_layout(
    struct inf_context *ctx, const char *n)
{
    struct cfg_m_str_m_str_pair_options *iter;
    for(iter = ctx->layouts; iter; iter = iter->next) {
        if(!strcmp(iter->key, n))
            return iter;
    }
    inf_report_error(ctx, "Bad layout name \"%s\"\n", n);
    return NULL;
}
