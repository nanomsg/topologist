#ifndef H_TOPOLOGY
#define H_TOPOLOGY

#include "hash_tables.h"

struct role_rules {
};

struct topology {
    struct roleht roles;
};

struct graph {
    struct topht topologies;
};


#endif  /* H_TOPOLOGY */
