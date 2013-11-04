#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "build.h"
#include "config.h"
#include "topology.h"
#include "infer/common.h"
#include "infer/bidi.h"
#include "infer/bus.h"
#include "infer/extern.h"

struct graph *graph_build(struct cfg_main *cfg) {
    struct graph *self = malloc(sizeof(struct graph));
    if(!self)
        return NULL;
    topht_init(&self->topologies);

    struct cfg_m_str_topology *iter;
    for(iter = cfg->topologies; iter; iter = iter->next) {
        struct inf_context ctx;
        ctx.layouts = cfg->layouts;
        struct topology *topology = NULL;
        switch(iter->val->any.tag) {
        case CFG_TOPOLOGY_TOPOLOGY:
            switch(iter->val->Topology.val.type) {
            case CFG_TOPOLOGY_TYPE_REQREP:
            case CFG_TOPOLOGY_TYPE_PIPELINE:
            case CFG_TOPOLOGY_TYPE_PUBSUB:
            case CFG_TOPOLOGY_TYPE_SURVEY:
                topology = infer_bidi_topology(&iter->val->Topology.val, &ctx);
                break;
            case CFG_TOPOLOGY_TYPE_BUS:
                topology = infer_bus_topology(&iter->val->Topology.val, &ctx);
                break;
            case CFG_TOPOLOGY_TYPE_PAIR:
                fprintf(stderr, "The pair topology is not supported yet\n");
                abort();
            default:
                fprintf(stderr, "Topologies type is not supported\n");
                abort();
            }
            break;
        case CFG_TOPOLOGY_EXTERN:
            topology = infer_extern_topology(&iter->val->Extern.val, &ctx);
            break;
        default:
            fprintf(stderr, "Wrong topology kind\n");
            abort();
        }
        if(topology)
            topht_set(&self->topologies, iter->key, topology);
    }

    return self;
}

void graph_free(struct graph *self) {
    topht_free(&self->topologies);
    free(self);
}
