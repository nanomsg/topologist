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
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "hash_tables.h"

static int random_value;

static uint32_t string_hash(const char *val) {
    const char *c;
    int32_t hash = random_value;
    for(c = val; *c; ++c) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*c);
    }
    if(!hash)
        hash = 1;
    return hash;
}

/*  Topology hash  */

#define HT_NAME(suffix) topht ## suffix
#define HT_STRUCT struct topht
#define HT_KEY_TYPE const char *
#define HT_HASH_FUNC string_hash
#define HT_KEY_COMPARE !strcmp
#define HT_VALUE_TYPE struct topology *
#define HT_VALUE_NOTHING NULL

#include "hash_table.c"

#undef HT_NAME
#undef HT_STRUCT
#undef HT_KEY_TYPE
#undef HT_HASH_FUNC
#undef HT_KEY_COMPARE
#undef HT_VALUE_TYPE
#undef HT_VALUE_NOTHING


/*  Role hash  */

#define HT_NAME(suffix) roleht ## suffix
#define HT_STRUCT struct roleht
#define HT_KEY_TYPE const char *
#define HT_HASH_FUNC string_hash
#define HT_KEY_COMPARE !strcmp
#define HT_VALUE_TYPE struct role *
#define HT_VALUE_NOTHING NULL

#include "hash_table.c"

#undef HT_NAME
#undef HT_STRUCT
#undef HT_KEY_TYPE
#undef HT_HASH_FUNC
#undef HT_KEY_COMPARE
#undef HT_VALUE_TYPE
#undef HT_VALUE_NOTHING
