#include <assert.h>
#include <stdlib.h>

#include "config.h"
#include "error.h"
#include "build.h"

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
