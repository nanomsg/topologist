/*
    Copyright (c) 2013 Insollo Entertainment, LLC.  All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

#include <malloc.h>
#include <string.h>

#include "bidi.h"
#include "utils.h"
#include "roles.h"
#include "../topology.h"


struct topology *infer_bidi_topology(
    struct cfg_intern *def, struct inf_context *ctx)
{
    struct cfg_m_str_a_str *layout;
    layout = inf_get_layout(ctx, def->layout);
    if(!layout) {
        return NULL;
    }
    struct topology *self = topology_new();
    if(!self)
        return NULL;
    self->default_port = def->port;

    struct cfg_a_str *conn;
    for(conn = layout->val; conn; conn = conn->next) {
        char bindrole[25];
        char connrole[25];
        int source;
        if(sscanf(conn->val, "%24s -> %24s", bindrole, connrole) == 2) {
            source = 0;
        } else if(sscanf(conn->val, "%24s <- %24s", bindrole, connrole) == 2) {
            source = 1;
        } else {
            err_add_fatal(&ctx->err, "Bad layout rule \"%s\"\n", conn->val);
            continue;
        }

        struct role *bindr = roleht_get(&self->roles, bindrole);
        if(!bindr) {
            bindr = malloc(sizeof(struct role));
            if(!bindr)
                goto memory_error;
            memcpy(bindr->name, bindrole, sizeof(bindrole));
            rrules_init(&bindr->source_rules);
            rrules_init(&bindr->sink_rules);
            if(roleht_set(&self->roles, bindr->name, bindr) < 0) {
                free(bindr);
                goto memory_error;
            }
        }

        struct role *connr = roleht_get(&self->roles, connrole);
        if(!connr) {
            connr = malloc(sizeof(struct role));
            if(!connr)
                goto memory_error;
            memcpy(connr->name, connrole, sizeof(connrole));
            rrules_init(&connr->source_rules);
            rrules_init(&connr->sink_rules);
            if(roleht_set(&self->roles, connr->name, connr) < 0) {
                free(connr);
                goto memory_error;
            }
        }

        struct role_endpoint *ep1;
        struct role_endpoint *ep2;
        if(source == 0) {
            ep1 = rrules_add_endpoint(&bindr->source_rules, 0);
            ep2 = rrules_add_endpoint(&connr->sink_rules, 1);
        } else {
            ep1 = rrules_add_endpoint(&bindr->sink_rules, 0);
            ep2 = rrules_add_endpoint(&connr->source_rules, 1);
        }
        ep1->peer = ep2;
        ep2->peer = ep1;
    }

    struct cfg_m_str_a_str *roleip;
    for(roleip = def->ip_addresses; roleip; roleip = roleip->next) {
        struct role *role = roleht_get(&self->roles, roleip->key);
        if(!role) {
            err_add_fatal(&ctx->err,
                "Assignment for non-existent role \"%s\"\n",
                roleip->key);
            continue;
        }

        struct cfg_a_str *ip;
        for(ip = roleip->val; ip; ip = ip->next) {
            if(role_add_ip(role, ip->val) < 0)
                goto memory_error;
        }
    }

    /* TODO(tailhook) check that all endpoints to bind are assigned */

    return self;
memory_error:
    topology_free(self);
    return NULL;
}
