#include "stdlib.h"

#include "build.h"
#include "topology.h"
#include "config.h"

struct graph *graph_build(struct cfg_main *cfg) {
    struct graph *self = malloc(sizeof(struct graph));
    if(!self)
        return NULL;
    topht_init(&self->topologies);

    struct cfg_m_str_topology *iter;
    for(iter = cfg->topologies; iter; iter = iter->next) {
        printf("TOPOLOGY ``%s''\n", iter->key);
    }

    return self;
}

void graph_free(struct graph *self) {
    topht_free(&self->topologies);
    free(self);
}
