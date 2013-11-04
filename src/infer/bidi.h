#ifndef H_INFER_BIDI
#define H_INFER_BIDI

#include "config.h"
#include "common.h"

struct topology *infer_bidi_topology(
    struct cfg_intern *def, struct inf_context *ctx);

#endif  /* H_INFER_BIDI */
