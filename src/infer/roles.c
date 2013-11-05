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

#include <errno.h>

#include "roles.h"

void rrules_init(struct role_rules *self) {
    self->head = NULL;
    self->tail = &self->head;
}

struct role_endpoint *rrules_add_endpoint(struct role_rules *self,
    struct cfg_pair_options *opt, int connect)
{
    struct role_endpoint *ep = malloc(sizeof(struct role_endpoint));
    if(!ep)
        return NULL;
    ep->next = NULL;
    *self->tail = ep;
    self->tail = &ep->next;
    ep->opt = opt;
    ep->connect = connect;
    ep->assign_head = NULL;
    ep->assign_tail = &ep->assign_head;
    ep->peer = NULL;
    return ep;
}

struct role_assign *ep_add_assign(struct role_endpoint *self,
                                  struct cfg_assignment *ass)
{
    struct role_assign *sass = malloc(sizeof(struct role_assign));
    if(!sass)
        return NULL;
    sass->val = ass;
    sass->next = NULL;
    *self->assign_tail = sass;
    self->assign_tail = &sass->next;
    return sass;
}

int role_add_assign(struct role *self, struct cfg_assignment *ass)
{
    struct role_endpoint *ep;
    for(ep = self->source_rules.head; ep; ep = ep->next) {
        if(!ep->connect) {
            if(!ep_add_assign(ep, ass))
                return -ENOMEM;
        }
    }
    for(ep = self->sink_rules.head; ep; ep = ep->next) {
        if(!ep->connect) {
            if(!ep_add_assign(ep, ass))
                return -ENOMEM;
        }
    }
    return 0;
}
