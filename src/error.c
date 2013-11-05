/*
    Copyright (c) 2013 Insollo Entertainment, LLC.  All rights reserved.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom
    the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/
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
    if(self->empty)
        return;
    fputs(self->buf, stream);
}
