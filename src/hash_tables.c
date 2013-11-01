#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "hash_tables.h"

#include "hash_table.h"

#define HT_NAME(suffix) topht ## suffix
#define HT_STRUCT struct topht
#define HT_KEY_TYPE const char *
#define HT_HASH_FUNC string_hash
#define HT_KEY_COMPARE !strcmp
#define HT_VALUE_TYPE struct topology *
#define HT_VALUE_NOTHING NULL

static int random_value;

static int32_t string_hash(const char *val) {
    const char *c;
    int32_t hash = random_value;
    for(c = val; *c; ++c) {
        hash = ((hash << 5) ^ (hash >> 27)) ^ (*c);
    }
    if(!hash)
        hash = 1;
    return hash;
}

#include "hash_table.c"

