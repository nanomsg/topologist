#include <malloc.h>

#include "bidi.h"
#include "utils.h"
#include "roles.h"
#include "../topology.h"


struct topology *infer_bidi_topology(
    struct cfg_intern *def, struct inf_context *ctx)
{
    struct topology *self = malloc(sizeof(struct topology));
    struct cfg_m_str_m_str_pair_options *layout;
    layout = inf_get_layout(ctx, def->layout);
    if(!layout) {
        free(self);
        return NULL;
    }
    roleht_init(&self->roles);

    struct cfg_m_str_pair_options *conn;
    for(conn = layout->val; conn; conn = conn->next) {
        char bindrole[25];
        char connrole[25];
        int source;
        if(sscanf(conn->key, "%24s -> %24s", bindrole, connrole) == 2) {
            source = 0;
        } else if(sscanf(conn->key, "%24s <- %24s", bindrole, connrole) == 2) {
            source = 1;
        } else {
            err_add_fatal(&ctx->err, "Bad layout rule \"%s\"\n", conn->key);
            continue;
        }

        struct role *bindr = roleht_get(&self->roles, bindrole);
        if(!bindr) {
            bindr = malloc(sizeof(struct role));
            bindr->name = bindrole;
            rrules_init(&bindr->source_rules);
            rrules_init(&bindr->sink_rules);
            roleht_set(&self->roles, bindrole, bindr);
        }

        struct role *connr = roleht_get(&self->roles, bindrole);
        if(!connr) {
            connr = malloc(sizeof(struct role));
            connr->name = connrole;
            rrules_init(&connr->source_rules);
            rrules_init(&connr->sink_rules);
            roleht_set(&self->roles, connrole, connr);
        }

        struct role_endpoint *ep1;
        struct role_endpoint *ep2;
        if(source == 0) {
            ep1 = rrules_add_endpoint(&bindr->source_rules, &conn->val, 0);
            ep2 = rrules_add_endpoint(&connr->sink_rules, &conn->val, 1);
        } else {
            ep1 = rrules_add_endpoint(&bindr->sink_rules, &conn->val, 0);
            ep2 = rrules_add_endpoint(&connr->source_rules, &conn->val, 1);
        }
        ep1->peer = ep2;
        ep2->peer = ep1;
    }

    struct cfg_m_str_a_assignment *roleass;
    for(roleass = def->assign; roleass; roleass = roleass->next) {
        struct role *role = roleht_get(&self->roles, roleass->key);
        if(!role) {
            err_add_fatal(&ctx->err,
                "Assignment for non-existent role \"%s\"\n",
                roleass->key);
            continue;
        }

        struct cfg_a_assignment *ass;
        for(ass = roleass->val; ass; ass = ass->next) {
            role_add_assign(role, &ass->val);
        }
    }

    /* TODO(tailhook) check that all endpoints to bind are assigned */

    return self;
}
