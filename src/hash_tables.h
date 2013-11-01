#ifndef H_HASH_TABLES
#define H_HASH_TABLES

#include <stdint.h>

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
void topht_free(struct topht *table);

#endif  /*  H_HASH_TABLES  */
