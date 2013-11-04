#ifndef H_TOPOLOGY
#define H_TOPOLOGY

#include "hash_tables.h"

struct role_assign {
    struct role_assign *next;
    struct cfg_assignment *val;
};

struct role_endpoint {
    struct role_endpoint *next;
    struct cfg_pair_options *opt;
    int connect;
    struct role_endpoint *peer;

    struct role_assign *assign_head;
    struct role_assign **assign_tail;
};

struct role_rules {
    struct role_endpoint *head;
    struct role_endpoint **tail;
};

struct role {
    char *name;
    struct role_rules source_rules;
    struct role_rules sink_rules;
};

struct topology {
    struct roleht roles;
};

struct graph {
    struct topht topologies;
};


#endif  /* H_TOPOLOGY */
