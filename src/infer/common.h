#ifndef H_INFER_COMMON
#define H_INFER_COMMON

#include "config.h"
#include "../error.h"

struct inf_context {
    struct cfg_m_str_m_str_pair_options *layouts;
    struct errbuf err;
};

#endif  /* H_INFER_COMMON */
