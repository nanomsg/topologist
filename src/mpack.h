#ifndef H_MPACK
#define H_MPACK

#include "hints.h"

struct mp_buf {
    char *buf;
    char *ptr;
    char *bufend;
};

#define MP_CHECK(ctx, mpbuf, funccall) do { \
    int rc = (funccall); \
    if(rc < 0) {\
        err_add_fatal(&ctx->err, \
            "Error serializing result: %s", strerror(-rc)); \
        mp_free((mpbuf)); \
        return rc; \
    } \
    } while(0);

int mp_init(struct mp_buf *buf) TT_USE_RESULT;
int mp_start_array(struct mp_buf *buf, int size) TT_USE_RESULT;
int mp_start_map(struct mp_buf *buf, int size) TT_USE_RESULT;
int mp_put_int(struct mp_buf *buf, long val) TT_USE_RESULT;
int mp_put_string(struct mp_buf *buf, const char *val, int len) TT_USE_RESULT;
void mp_free(struct mp_buf *buf);
void mp_move_to(struct mp_buf *buf, const char **result, int *resultlen);

#endif  /* H_MPACK */
