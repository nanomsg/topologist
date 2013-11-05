#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "query.h"
#include "build.h"
#include "config.h"

static void query_topology(struct cfg_main *cfg) {
    int rc;
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
    if(cfg->output && strcmp(cfg->output, "-")) {
        FILE *f = fopen(cfg->output, "w");
        if(!f) {
            fprintf(stderr, "Error opening file \"%s\": %s",
                cfg->output, strerror(errno));
            exit(1);
        }
        stdout = f;
    }
    if(cfg->query_file) {
        const char *databuf;
        int datalen;
        char linebuf[4096];
        FILE *inp = fopen(cfg->query_file, "r");

        if(!inp) {
            fprintf(stderr, "Error opening file \"%s\": %s",
                cfg->output, strerror(errno));
            exit(1);
        }

        while(fgets(linebuf, sizeof(linebuf)-1, inp)) {
            struct query_context ctx;
            int len = strlen(linebuf);
            if(linebuf[len-1] == '\n')
                --len;
            printf("%.*s\n", len, linebuf);
            rc = execute_query(&ctx, g, linebuf, len, &databuf, &datalen);
            if(rc < 0) {
                err_print(&ctx.err, stderr);
                continue;
            }
            /*  TODO(tailhook) print result  */
            printf("GOT RESULT %d bytes\n", datalen);
            free((void *)databuf);
        }

        fclose(inp);

    }
    fclose(stdout);
    graph_free(g);
}

int main(int argc, char **argv) {
    int rc;
    struct cfg_main cfg;

    rc = cfg_load(&cfg, argc, argv);
    if(rc == 0) {
        query_topology(&cfg);
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
