#ifndef H_INFER_BIDI
#define H_INFER_BIDI

#include "config.h"
#include "common.h"
#include "../hints.h"

struct topology *infer_bidi_topology(
    struct cfg_intern *def, struct inf_context *ctx) TT_USE_RESULT;

#endif  /* H_INFER_BIDI */
