#ifndef H_INFER_ROLES
#define H_INFER_ROLES

#include "../topology.h"
#include "config.h"
#include "../hints.h"

void rrules_init(struct role_rules *self);
struct role_endpoint *rrules_add_endpoint(struct role_rules *self,
    struct cfg_pair_options *opt, int connect) TT_USE_RESULT;
int role_add_assign(struct role *self, struct cfg_assignment *ass)
    TT_USE_RESULT;

#endif  /* H_INFER_ROLES */
