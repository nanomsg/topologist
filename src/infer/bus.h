#ifndef H_INFER_BUS
#define H_INFER_BUS

#include "config.h"
#include "common.h"

struct topology *infer_bus_topology(
    struct cfg_intern *def, struct inf_context *ctx);

#endif  /* H_INFER_BUS */
