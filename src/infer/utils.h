#ifndef H_INFER_UTILS
#define H_INFER_UTILS

#include "common.h"

void inf_report_error(struct inf_context *ctx, const char *fmt, ...);
struct cfg_m_str_m_str_pair_options *inf_get_layout(
    struct inf_context *ctx, const char *n);


#endif  /* H_INFER_UTILS */
