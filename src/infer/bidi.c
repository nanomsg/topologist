#include <malloc.h>

#include "bidi.h"
#include "utils.h"
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
            source = 1;
        } else if(sscanf(conn->key, "%24s <- %24s", bindrole, connrole) == 2) {
            source = 2;
        } else if(sscanf(conn->key, "%24s -- %24s", bindrole, connrole) == 2) {
            source = 0;
        } else {
            inf_report_error(ctx, "Bad layout rule \"%s\"\n", conn->key);
        }
        /*  TODO(tailhook) check if 'source' is compatible with a pattern  */
    }
    return self;
}
