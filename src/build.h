#ifndef H_BUILD
#define H_BUILD

struct topology;
struct cfg_main;
struct errbuf;

struct graph *graph_build(struct cfg_main *cfg, struct errbuf *err);
void graph_free(struct graph *g);

#endif  /* H_BUILD */
