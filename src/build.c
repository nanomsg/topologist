#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "build.h"
#include "topology.h"
#include "config.h"

struct build_context {
    struct cfg_m_str_m_str_pair_options *layouts;
};

static void report_error(struct build_context *ctx, const char *fmt, ...) {
    (void) ctx;  /* Build context */
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

static struct cfg_m_str_m_str_pair_options *get_layout(
    struct build_context *ctx, const char *n)
{
    struct cfg_m_str_m_str_pair_options *iter;
    for(iter = ctx->layouts; iter; iter = iter->next) {
        if(!strcmp(iter->key, n))
            return iter;
    }
    report_error(ctx, "Bad layout name \"%s\"\n", n);
    return NULL;
}

static struct topology *topology_build(
    struct cfg_intern *def, struct build_context *ctx)
{
    struct topology *self = malloc(sizeof(struct topology));
    struct cfg_m_str_m_str_pair_options *layout = get_layout(ctx, def->layout);
    if(!layout) {
        free(self);
        return NULL;
    }
    roleht_init(&self->roles);
    struct cfg_m_str_pair_options *conn;
    for(conn = layout->val; conn; conn = conn->next) {
        char bindrole[24];
        char connrole[24];
        int source;
        if(sscanf(conn->key, "%23s -> %23s", bindrole, connrole) == 2) {
            source = 1;
        } else if(sscanf(conn->key, "%23s <- %23s", bindrole, connrole) == 2) {
            source = 2;
        } else if(sscanf(conn->key, "%23s -- %23s", bindrole, connrole) == 2) {
            source = 0;
        } else {
            report_error(ctx, "Bad layout rule \"%s\"\n", conn->key);
        }
        /*  TODO(tailhook) check if 'source' is compatible with a pattern  */
    }
    return self;
}

static struct topology *topology_extract(
    struct cfg_extern *def, struct build_context *ctx)
{
    (void) def; (void) ctx;
    struct topology *self = malloc(sizeof(struct topology));
    //struct topology *source = get_topology(ctx, def->name);
    fprintf(stderr, "Extern topologies are not implemented yet\n");
    abort();
    return self;
}

struct graph *graph_build(struct cfg_main *cfg) {
    struct graph *self = malloc(sizeof(struct graph));
    if(!self)
        return NULL;
    topht_init(&self->topologies);

    struct cfg_m_str_topology *iter;
    for(iter = cfg->topologies; iter; iter = iter->next) {
        struct build_context ctx;
        ctx.layouts = cfg->layouts;
        struct topology *topology = NULL;
        switch(iter->val->any.tag) {
            case CFG_TOPOLOGY_TOPOLOGY:
                topology = topology_build(&iter->val->Topology.val, &ctx);
                break;
            case CFG_TOPOLOGY_EXTERN:
                topology = topology_extract(&iter->val->Extern.val, &ctx);
                break;
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
