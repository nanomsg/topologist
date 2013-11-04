#include "topology.h"

struct topology *topology_new() {
    struct topology *self = malloc(sizeof(struct topology));
    if(!self)
        return NULL;
    roleht_init(&self->roles);
    return self;
}

static void rrules_free(struct role_rules *rules) {
    struct role_endpoint *ep, *nxt;
    for(ep = rules->head; ep; ep = nxt) {
        nxt = ep->next;
        struct role_assign *acur, *anxt;
        for(acur = ep->assign_head; acur; acur = anxt) {
            anxt = acur->next;
            free(acur);
        }
        free(ep);
    }
}

static void role_free(struct role *role) {
    rrules_free(&role->source_rules);
    rrules_free(&role->sink_rules);
    free(role);
}

void topology_free(struct topology *self) {
    /* TODO(tailhook) free roles */
    roleht_free(&self->roles, role_free);
    free(self);
}
