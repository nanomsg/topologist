#ifndef H_INFER_EXTERN
#define H_INFER_EXTERN

#include "config.h"
#include "common.h"

struct topology *infer_extern_topology(
    struct cfg_extern *def, struct inf_context *ctx);

#endif  /* H_INFER_EXTERN */
