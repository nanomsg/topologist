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
int topht_set(struct topht *table, const char *key, struct topology *value);
struct topology *topht_get(struct topht *table, const char *key);
void topht_free(struct topht *table);


struct role_rules;

HASH_TABLE_STRUCT(roleht, const char *, struct role_rules *);
void roleht_init(struct roleht *table);
int roleht_set(struct roleht *table, const char *key, struct role_rules *value);
struct role_rules *roleht_get(struct roleht *table, const char *key);
void roleht_free(struct roleht *table);

#endif  /*  H_HASH_TABLES  */
