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

struct graph *graph_build(struct cfg_main *cfg, struct errbuf *err) {
    struct graph *self = malloc(sizeof(struct graph));
    if(!self)
        return NULL;
    topht_init(&self->topologies);

    struct inf_context ctx;
    err_init(&ctx.err);
    ctx.layouts = cfg->layouts;

    struct cfg_m_str_topology *iter;
    for(iter = cfg->topologies; iter; iter = iter->next) {
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
                err_add_fatal(&ctx.err,
                    "The pair topology is not supported yet\n");
                break;
            default:
                err_add_fatal(&ctx.err,
                    "Unsupported topology type\n");
                break;
            }
            break;
        case CFG_TOPOLOGY_EXTERN:
            topology = infer_extern_topology(&iter->val->Extern.val, &ctx);
            break;
        default:
            err_add_fatal(&ctx.err,
                "Unsupported topology kind\n");
            break;
        }
        if(topht_get(&self->topologies, iter->key)) {
            err_add_fatal(&ctx.err, "Duplicate topology \"%s\"\n", iter->key);
        }
        if(ctx.err.fatal) {
            if(topology)
                topology_free(topology);
        }
        if(topht_set(&self->topologies, iter->key, topology) < 0) {
            graph_free(self);
            return NULL;
        }
    }

    memcpy(err, &ctx.err,  sizeof(struct errbuf));
    return self;
}

void graph_free(struct graph *self) {
    topht_free(&self->topologies, topology_free);
    free(self);
}
