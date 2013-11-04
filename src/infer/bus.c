#include <malloc.h>
#include <stdlib.h>

#include "bus.h"
#include "../topology.h"


struct topology *infer_bus_topology(
    struct cfg_intern *def, struct inf_context *ctx)
{
    (void) def; (void) ctx;
    struct topology *self = malloc(sizeof(struct topology));
    fprintf(stderr, "Bus topologies are not implemented yet\n");
    abort();
    return self;
}
