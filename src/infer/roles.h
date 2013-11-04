#ifndef H_INFER_ROLES
#define H_INFER_ROLES

#include "../topology.h"
#include "config.h"

void rrules_init(struct role_rules *self);
struct role_endpoint *rrules_add_endpoint(struct role_rules *self,
    struct cfg_pair_options *opt, int connect);
void role_add_assign(struct role *self, struct cfg_assignment *ass);

#endif  /* H_INFER_ROLES */
