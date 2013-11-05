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
#include <errno.h>

void HT_NAME(_init)(HT_STRUCT *table) {
    table->table_size = HT_DENSE_SIZE_LOG;
    table->table = NULL;
    table->len = 0;
    memset(table->dense, 0, sizeof(table->dense));
}

void HT_NAME(_free)(HT_STRUCT *table, void (*free_elm)(HT_VALUE_TYPE elm)) {
    uint32_t i;
    struct HT_NAME(_entry) *tab;
    uint32_t tsize = 1 << table->table_size;

    if(free_elm) {
        if(table->table) {
            tab = table->table;
        } else {
            tab = table->dense;
        }
        for(i = 0; i < tsize; ++i) {
            if(tab[i].hash) {
                free_elm(tab[i].value);
            }
        }
    }

    if(table->table)
        free(table);
}

static struct HT_NAME(_entry) *HT_NAME(_find)(HT_STRUCT *table,
    uint32_t hash, HT_KEY_TYPE key)
{
    int i;
    struct HT_NAME(_entry) *tab;
    struct HT_NAME(_entry) *e;
    uint32_t tmask = (1 << table->table_size) - 1;

    if(table->table) {
        tab = table->table;
    } else {
        tab = table->dense;
    }

    uint32_t cell = hash;
    for(i = 0;; ++i) {
        cell = (cell + i) & tmask;
        e = &tab[cell];
        if(!e->hash || (e->hash == hash && HT_KEY_COMPARE(e->key, key)))
            return e;
    }
    abort();
}

int HT_NAME(_resize)(HT_STRUCT *table, size_t newsize) {
    struct HT_NAME(_entry) *otab;
    if(table->table) {
        otab = table->table;
    } else {
        otab = table->dense;
    }
    struct HT_NAME(_entry) *entry, *nentry;
    entry = otab + (1 << table->table_size) - 1;

    table->table = malloc(sizeof(*otab) * newsize);
    if(!table->table)
        return -ENOMEM;
    table->table_size = newsize;
    memset(table->table, 0, newsize*sizeof(*otab));

    while(entry > otab) {
        nentry = HT_NAME(_find)(table, entry->hash, entry->key);
        nentry->hash = entry->hash;
        nentry->key = entry->key;
        nentry->value = entry->value;
    }

    if(otab != table->table)
        free(otab);
    return 0;
}

int HT_NAME(_set)(HT_STRUCT *table, HT_KEY_TYPE key, HT_VALUE_TYPE val)
{
    int rc;
    uint32_t hash = HT_HASH_FUNC(key);
    struct HT_NAME(_entry) *entry;
    entry = HT_NAME(_find)( table, hash, key);
    if(entry->hash) {
        /*  Key already in the table, just replace the value  */
        entry->value = val;
        return 0;
    }
    size_t tsize = table->table_size;
    table->len += 1;
    if((tsize * 3 / 4) > table->len) {
        entry->hash = hash;
        entry->key = key;
        entry->value = val;
    } else {
        rc = HT_NAME(_resize)(table, table->table_size + 1);
        if(rc)
            return rc;
        entry = HT_NAME(_find)(table, hash, key);
        entry->hash = hash;
        entry->key = key;
        entry->value = val;
    }
    return 1;
}

HT_VALUE_TYPE HT_NAME(_get)(HT_STRUCT *table, HT_KEY_TYPE key) {
    static struct HT_NAME(_entry) *entry;
    uint32_t hash = HT_HASH_FUNC(key);
    entry = HT_NAME(_find)( table, hash, key);
    if(entry->hash)
        return entry->value;
    return HT_VALUE_NOTHING;
}
