#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "../error.h"

struct cfg_m_str_m_str_pair_options *inf_get_layout(
    struct inf_context *ctx, const char *n)
{
    struct cfg_m_str_m_str_pair_options *iter;
    for(iter = ctx->layouts; iter; iter = iter->next) {
        if(!strcmp(iter->key, n))
            return iter;
    }
    err_add_fatal(&ctx->err, "Bad layout name \"%s\"\n", n);
    return NULL;
}
