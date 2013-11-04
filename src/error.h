#ifndef TOPOLOGIST_H_ERROR
#define TOPOLOGIST_H_ERROR

#include "hints.h"

struct errbuf {
    char buf[4096];
    int len;
    int empty:1;
    int fatal:1;
};

void err_init(struct errbuf *buf);
void err_add_fatal(struct errbuf *buf, const char *fmt, ...) TT_FORMAT(2);
void err_add_warning(struct errbuf *buf, const char *fmt, ...) TT_FORMAT(2);
void err_print(struct errbuf *self, FILE *stream);

#endif  /* TOPOLOGIST_H_ERROR */
