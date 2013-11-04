#ifndef H_QUERY
#define H_QUERY

#include "config.h"
#include "topology.h"
#include "error.h"

struct query {
    char topology[65];
    char role[25];
    int is_source:1;
    int socket_type;
};

struct query_context {
    struct query query;
    struct errbuf err;
};

int execute_query(struct query_context *ctx, struct graph *g,
    const char *buf, int buflen,
    const char **result, int *resultlen);

#endif  /* H_QUERY */
