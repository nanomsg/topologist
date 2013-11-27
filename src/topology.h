#ifndef H_TOPOLOGY
#define H_TOPOLOGY

#include "hints.h"
#include "hash_tables.h"

struct role_ip {
    struct role_ip *next;
    const char *ip;
};

struct role_endpoint {
    struct role_endpoint *next;
    int connect;
    struct role_endpoint *peer;

    struct role_ip *ip_head;
    struct role_ip **ip_tail;
};

struct role_rules {
    struct role_endpoint *head;
    struct role_endpoint **tail;
};

struct role {
    char name[25];
    struct role_rules source_rules;
    struct role_rules sink_rules;
};

struct topology {
    struct roleht roles;
    int default_port;
    struct cfg_sockopts *default_options;
};

struct graph {
    struct topht topologies;
};

struct topology *topology_new() TT_USE_RESULT;
void topology_free(struct topology *);


#endif  /* H_TOPOLOGY */
