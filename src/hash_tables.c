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
