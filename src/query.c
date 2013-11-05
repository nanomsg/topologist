#include <string.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

#include "query.h"
#include "mpack.h"

const int min_request_size = 8 /* strlen("RESOLVE ") */ +
    15 /* strlen("nanoconfig://a ") */ +
    4 /* strlen("NN_x") */;

static struct socket_type {
    const char *name;
    int is_source;
    int sock_type;
} socket_types[] = {
    {"NN_REQ", 1, NN_REQ},
    {"NN_REP", 0, NN_REP},
    {NULL, 0, 0}
};

static int query_url_parse(struct query_context *ctx,
                           struct query *self,
                           const char *url, int urllen)
{
    /* Url starts by "nanoconfig://" it's enforced in query_parse */
    const char *topname = url + 13 /* strlen("nanoconfig://") */;
    const char *urlend = url + urllen;
    const char *qstr = memchr(topname, '?', urlend - topname);
    if(!qstr) {
        err_add_fatal(&ctx->err, "Role is required");
        return 0;
    }
    int toplen = qstr - topname;
    qstr += 1;
    if(toplen > 64) {
        err_add_fatal(&ctx->err, "Topology name is too long");
        return 0;
    }
    memcpy(self->topology, topname, toplen);
    self->topology[toplen] = 0;
    self->role[0] = 0;
    self->ip[0] = 0;

    const char *eq, *amp;
    while(1) {
        eq = memchr(qstr, '=', urlend - qstr);
        amp = memchr(qstr, '&', urlend - qstr);
        const char *val;
        int vallen;
        if(eq) {
            val = eq + 1;
            if(amp) {
                vallen = amp - val;
            } else {
                vallen = urlend - val;
            }
        } else {
            eq = urlend;
            val = "";
            vallen = 0;
        }
        char *target = NULL;
        int maxlen = 0;

        if(eq - qstr == 4 && !memcmp(qstr, "role", 4)) {
            target = self->role;
            maxlen = 24;
        } else if(eq - qstr == 2 && !memcmp(qstr, "ip", 2)) {
            target = self->ip;
            maxlen = 39;
        }

        if(target && maxlen) {
            if(vallen > maxlen) {
                err_add_warning(&ctx->err, "Field \"%.*s\" truncated",
                    vallen, val);
            }
            memcpy(target, val, vallen);
            target[vallen] = 0;
        }
        if(amp) {
            qstr = amp+1;
        } else {
            break;
        }
    }
    if(!self->role[0]) {
        err_add_fatal(&ctx->err, "Role must be non-empty");
        return 0;
    }
    return 1;
}

static int query_parse(struct query_context *ctx, const char *q, int qlen)
{
    const char *qend = q + qlen;
    if(qlen < min_request_size) {
        err_add_fatal(&ctx->err, "Query too short");
        return 0;
    }
    if(memcmp("RESOLVE ", q, 8)) {
        err_add_fatal(&ctx->err, "Query must start with RESOLVE");
        return 0;
    }
    const char *urlstart = q + 8;
    const char *urlend = memchr(urlstart, ' ', qend - urlstart);
    if(memcmp("nanoconfig://", urlstart, 13)) {
        fprintf(stderr, "URLSTART \"%s\"\n", urlstart);
        err_add_fatal(&ctx->err, "Url must start with nanoconfig://");
        return 0;
    }
    if(!urlend) {
        err_add_fatal(&ctx->err, "Query must consist of socket type");
        return 0;
    }
    const char *socktype = urlend + 1;
    int socktypelen = qend - socktype;
    if(socktypelen < 4 && memcmp("NN_", socktype, 3)) {
        err_add_fatal(&ctx->err, "Socket type should be NN_something");
        return 0;
    }

    int type = 0;
    int is_source = -1;
    const struct socket_type *typ;
    for(typ = socket_types; typ->name; ++typ) {
        if((int)strlen(typ->name) == socktypelen &&
            !strncmp(typ->name, socktype, socktypelen)) {
            is_source = typ->is_source;
            type = typ->sock_type;
            break;
        }
    }
    if(type == 0) {
        err_add_fatal(&ctx->err, "Socket type \"%.*s\" is unknown",
            socktypelen, socktype);
        return 0;
    }
    struct query *self = &ctx->query;
    self->socket_type = type;
    self->is_source = is_source;
    return query_url_parse(ctx, self, urlstart, urlend-urlstart);
}

static int rrules_resolve(struct query_context *ctx, struct query *query,
    struct role_rules *rr, const char **result, int *resultlen)
{
    (void) ctx; (void) query; (void) result; (void) resultlen; (void) rr;
    struct mp_buf mp;
    struct role_endpoint *ep;
    struct role_assign *asg;
    struct cfg_pair_options *opt;

    MP_CHECK(ctx, &mp, mp_init(&mp))
    MP_CHECK(ctx, &mp, mp_start_array(&mp, 4));
    MP_CHECK(ctx, &mp, mp_put_int(&mp, 1));

    /*  TODO(tailhook) implement socket-level socket options  */
    MP_CHECK(ctx, &mp, mp_start_map(&mp, 0));

    /*  The msgpack requires to know size of mapping in advance
     *  so we first calculate it. Keep in sync with the code below */
    int epnum = 0;
    for(ep = rr->head; ep; ep = ep->next) {
        if(ep->connect) {
            if(ep->opt->port) {
                for(asg = ep->peer->assign_head; asg; asg = asg->next)
                    epnum += 1;
            } else if(ep->opt->local_addr) {
                epnum += 1;
            }
        } else {
            epnum += 1;
        }
    }

    MP_CHECK(ctx, &mp, mp_start_array(&mp, epnum));
    /*  The msgpack requires to know size of mapping in advance
     *  so we first calculate it. Keep in sync with the code above */
    for(ep = rr->head; ep; ep = ep->next) {
        MP_CHECK(ctx, &mp, mp_start_array(&mp, 3));
        MP_CHECK(ctx, &mp, mp_put_int(&mp, ep->connect));
        char addrbuf[128];  /* max nanomsg addr */
        int addrlen;
        if(ep->connect) {
            struct role_endpoint *pep = ep->peer;
            opt = pep->opt;
            if(opt->port) {
                for(asg = pep->assign_head; asg; asg = asg->next) {
                    addrlen = snprintf(addrbuf, sizeof(addrbuf)-1,
                        "tcp://%s:%d", asg->val->host, (int)opt->port);
                    MP_CHECK(ctx, &mp, mp_put_string(&mp, addrbuf, addrlen));
                }
            } else if(opt->local_addr) {
                MP_CHECK(ctx, &mp, mp_put_string(&mp,
                    opt->local_addr, opt->local_addr_len));
            }
        } else {
            opt = ep->opt;
            if(opt->port) {
                if(!query->ip[0]) {
                    mp_free(&mp);
                    err_add_fatal(&ctx->err,
                        "Peers with addressses to bind should provide "
                        "\"ip\" variable in topology url");
                    return -EADDRNOTAVAIL;
                }
                for(asg = ep->assign_head; asg; asg = asg->next) {
                    if(!strcmp(asg->val->host, query->ip)) {
                        addrlen = snprintf(addrbuf, sizeof(addrbuf)-1,
                            "tcp://%s:%d", asg->val->host, (int)opt->port);
                        MP_CHECK(ctx, &mp, mp_put_string(&mp, addrbuf, addrlen));
                        break;
                    }
                }
                if(!asg) {
                    mp_free(&mp);
                    err_add_fatal(&ctx->err,
                        "Role specifies that address should be bound to, but"
                        " no such address in \"assign\" clause.");
                    return -EADDRNOTAVAIL;
                }
            } else {
                MP_CHECK(ctx, &mp, mp_put_string(&mp,
                    opt->local_addr, opt->local_addr_len));
            }
        }
    }

    MP_CHECK(ctx, &mp, mp_start_array(&mp, 0));

    mp_move_to(&mp, result, resultlen);

    return 0;
}

int execute_query(struct query_context *ctx, struct graph *g,
    const char *query, int querylen,
    const char **result, int *resultlen)
{
    err_init(&ctx->err);
    if(!query_parse(ctx, query, querylen))
        return 0;
    struct query *q = &ctx->query;

    struct topology *top = topht_get(&g->topologies, q->topology);
    if(!top) {
        err_add_fatal(&ctx->err, "Topology \"%s\" not found",
            q->topology);
        return 0;
    }
    struct role *role = roleht_get(&top->roles, q->role);
    if(!role) {
        err_add_fatal(&ctx->err, "Role not found");
        return -ENOENT;
    }
    struct role_rules *rules = &role->source_rules;
    if(!q->is_source) {
        rules = &role->sink_rules;
    }
    return rrules_resolve(ctx, q, rules, result, resultlen);
}
