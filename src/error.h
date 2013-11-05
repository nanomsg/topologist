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
