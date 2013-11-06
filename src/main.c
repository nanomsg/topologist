#include <assert.h>
#include <stdlib.h>

#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <nanomsg/pubsub.h>

#include "config.h"
#include "error.h"
#include "build.h"
#include "query.h"
#include "mpack.h"

struct topologist {
    int response_socket;
    int updates_socket;
};

static void apply_socket_config(int sock, struct cfg_a_nanomsg_addr *addr) {
    int rc;

    for(; addr; addr = addr->next) {
        if(addr->val.tag == CFG_NANOMSG_ADDR_BIND) {
            rc = nn_bind(sock, addr->val.val);
            if(rc < 0) {
                fprintf(stderr, "Error in nn_bind: %s", nn_strerror(errno));
                exit(2);
            }
        } else if(addr->val.tag == CFG_NANOMSG_ADDR_CONNECT) {
            rc = nn_connect(sock, addr->val.val);
            if(rc < 0) {
                fprintf(stderr, "Error in nn_connect: %s", nn_strerror(errno));
                exit(2);
            }
        }
    }
}

static void topologist_loop (struct cfg_main *cfg, struct graph *g)
{
    struct topologist topologist;
    int rc;
    int qrc;

    topologist.response_socket = nn_socket(AF_SP, NN_REP);
    if(topologist.response_socket < 0) {
        fprintf(stderr, "Error opening socket: %s", nn_strerror(errno));
        exit(2);
    }
    apply_socket_config(topologist.response_socket, cfg->server.config_addr);

    topologist.updates_socket = nn_socket(AF_SP, NN_PUB);
    if(topologist.updates_socket < 0) {
        fprintf(stderr, "Error opening socket: %s", nn_strerror(errno));
        exit(2);
    }
    apply_socket_config(topologist.updates_socket, cfg->server.updates_addr);

    while (1) {
        char *buf;
        int buflen;

        buflen = nn_recv(topologist.response_socket, &buf, NN_MSG, 0);
        if(buflen == EAGAIN || buflen == EINTR)
            continue;

        const char *databuf;
        int datalen;
        struct query_context ctx;

        qrc = execute_query(&ctx, g, buf, buflen, &databuf, &datalen);
        if(qrc < 0) {
            struct mp_buf mp;
            if(mp_init(&mp) < 0
               || mp_start_array(&mp, 3) < 0
               || mp_put_int(&mp, 0) < 0
               || mp_put_int(&mp, -qrc) < 0
               || mp_put_string(&mp, ctx.err.buf, ctx.err.len) < 0)
            {
                fprintf(stderr,
                    "FATAL ERROR: Not enough memory to send message");
                exit(3);
            }
            continue;
        }

        while(1) {
            rc = nn_send(topologist.response_socket, databuf, datalen, 0);
            if(rc < 0) {
                if(errno == EAGAIN || errno == EINTR)
                    continue;
                fprintf(stderr, "FATAL ERROR: Can't send response: %s",
                    nn_strerror(errno));
                exit(4);
            }
            break;
        }

        /*  TODO(tailhook) print result  */
        free((void *)databuf);

    }
};

static void run_service(struct cfg_main *cfg) {
    struct errbuf err;
    struct graph *g = graph_build(cfg, &err);
    if(!g) {
        fprintf(stderr, "topology: Not enough memory to build graph");
        exit(1);
    }
    if(!err.empty) {
        err_print(&err, stderr);
        if(err.fatal) {
            graph_free(g);
            exit(1);
        }
    }

    topologist_loop(cfg, g);
}

int main(int argc, char **argv) {
    int rc;
    struct cfg_main cfg;

    rc = cfg_load(&cfg, argc, argv);
    if(rc == 0) {
        run_service(&cfg);
    }
    cfg_free(&cfg);

    if(rc > 0) {
        /*  rc > 0 means we had some configuration error  */
        return rc;
    } else {
        /*  rc == 0 means we have run successfully  */
        /*  rc < 0 means we've done some config action successfully  */
        return 0;
    }
}
