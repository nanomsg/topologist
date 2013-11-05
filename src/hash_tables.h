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
#ifndef H_HASH_TABLES
#define H_HASH_TABLES

#include <stdint.h>
#include <stdlib.h>
#include "hints.h"

/*  Abstract macros  */

#define HT_DENSE_SIZE 16
#define HT_DENSE_SIZE_LOG 4

#define HASH_TABLE_STRUCT(name, keytype, valuetype) \
    struct name ## _entry { \
        uint32_t hash; \
        keytype key; \
        valuetype value; \
    }; \
    struct name { \
        struct name ## _entry dense[HT_DENSE_SIZE]; \
        size_t table_size; \
        size_t len; \
        struct name ## _entry *table; \
    };


/*  Definitions  */

struct topology;

HASH_TABLE_STRUCT(topht, const char *, struct topology *);
void topht_init(struct topht *table);
int topht_set(struct topht *table, const char *key, struct topology *value)
    TT_USE_RESULT;
struct topology *topht_get(struct topht *table, const char *key);
void topht_free(struct topht *table, void (*free_elm)(struct topology *));


struct role;

HASH_TABLE_STRUCT(roleht, const char *, struct role *);
void roleht_init(struct roleht *table);
int roleht_set(struct roleht *table, const char *key, struct role *value)
    TT_USE_RESULT;
struct role *roleht_get(struct roleht *table, const char *key);
void roleht_free(struct roleht *table, void (*free_elm)(struct role *));

#endif  /*  H_HASH_TABLES  */
