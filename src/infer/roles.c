#include "roles.h"

void rrules_init(struct role_rules *self) {
    self->head = NULL;
    self->tail = &self->head;
}

struct role_endpoint *rrules_add_endpoint(struct role_rules *self,
    struct cfg_pair_options *opt, int connect)
{
    struct role_endpoint *ep = malloc(sizeof(struct role_endpoint));
    ep->next = NULL;
    *self->tail = ep;
    self->tail = &ep->next;
    ep->opt = opt;
    ep->connect = connect;
    ep->assign_head = NULL;
    ep->assign_tail = &ep->assign_head;
    ep->peer = NULL;
    return ep;
}

void ep_add_assign(struct role_endpoint *self, struct cfg_assignment *ass) {
    struct role_assign *sass = malloc(sizeof(struct role_assign));
    sass->val = ass;
    sass->next = NULL;
    *self->assign_tail = sass;
    self->assign_tail = &sass->next;
}

void role_add_assign(struct role *self, struct cfg_assignment *ass) {
    struct role_endpoint *ep;
    for(ep = self->source_rules.head; ep; ep = ep->next) {
        if(!ep->connect) {
            ep_add_assign(ep, ass);
        }
    }
    for(ep = self->sink_rules.head; ep; ep = ep->next) {
        if(!ep->connect) {
            ep_add_assign(ep, ass);
        }
    }
}
