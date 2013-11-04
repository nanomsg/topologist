#include <stdio.h>
#include <stdarg.h>

#include "error.h"

void err_init(struct errbuf *buf) {
    buf->len = 0;
    buf->empty = 1;
    buf->fatal = 0;
}

void err_add_fatal(struct errbuf *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int added = vsnprintf(buf->buf + buf->len, sizeof(buf->buf) - buf->len - 2,
        fmt, args);
    va_end(args);
    if(added > 0) {
        buf->len += added + 1;
        buf->buf[buf->len-1] = '\n';
        buf->buf[buf->len] = 0;
    }
    buf->empty = 0;
    buf->fatal = 1;
}

void err_add_warning(struct errbuf *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int added = vsnprintf(buf->buf + buf->len, sizeof(buf->buf) - buf->len - 2,
        fmt, args);
    va_end(args);
    if(added > 0) {
        buf->len += added + 1;
        buf->buf[buf->len-1] = '\n';
        buf->buf[buf->len] = 0;
    }
    buf->empty = 0;
}

void err_print(struct errbuf *self, FILE *stream) {
    fputs(self->buf, stream);
}
