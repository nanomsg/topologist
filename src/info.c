#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "infoconfig.h"

static void query_topology(struct cfg_main *cfg) {
    if(cfg->cli.output && strcmp(cfg->cli.output, "-")) {
        FILE *f = fopen(cfg->cli.output, "w");
        if(!f) {
            fprintf(stderr, "Error opening file \"%s\": %s",
                cfg->cli.output, strerror(errno));
            exit(1);
        }
        stdout = f;
    }
    if(cfg->cli.query_file) {
    }
    fclose(stdout);
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
