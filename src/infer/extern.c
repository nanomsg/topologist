#include <malloc.h>
#include <stdlib.h>

#include "extern.h"
#include "../topology.h"

struct topology *infer_extern_topology(
    struct cfg_extern *def, struct inf_context *ctx)
{
    (void) def; (void) ctx;
    struct topology *self = malloc(sizeof(struct topology));
    //struct topology *source = get_topology(ctx, def->name);
    fprintf(stderr, "Extern topologies are not implemented yet\n");
    abort();
    return self;
}
