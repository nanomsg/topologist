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

struct role_endpoint *rrules_add_endpoint(struct role_rules *self, int connect)
{
    struct role_endpoint *ep = malloc(sizeof(struct role_endpoint));
    if(!ep)
        return NULL;
    ep->next = NULL;
    *self->tail = ep;
    self->tail = &ep->next;
    ep->connect = connect;
    ep->ip_head = NULL;
    ep->ip_tail = &ep->ip_head;
    ep->peer = NULL;
    return ep;
}

struct role_ip *ep_add_ip(struct role_endpoint *self,
                                  const char *ip)
{
    struct role_ip *sip = malloc(sizeof(struct role_ip));
    if(!sip)
        return NULL;
    sip->ip = ip;
    sip->next = NULL;
    *self->ip_tail = sip;
    self->ip_tail = &sip->next;
    return sip;
}

int role_add_ip(struct role *self, const char *ip)
{
    struct role_endpoint *ep;
    for(ep = self->source_rules.head; ep; ep = ep->next) {
        if(!ep->connect) {
            if(!ep_add_ip(ep, ip))
                return -ENOMEM;
        }
    }
    for(ep = self->sink_rules.head; ep; ep = ep->next) {
        if(!ep->connect) {
            if(!ep_add_ip(ep, ip))
                return -ENOMEM;
        }
    }
    return 0;
}
