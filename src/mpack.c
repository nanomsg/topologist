#include <malloc.h>
#include <stdint.h>
#include <errno.h>
#include <endian.h>
#include <string.h>

#include "mpack.h"

int mp_init(struct mp_buf *buf) {
    buf->bufend = NULL;
    buf->buf = malloc(1024);
    buf->ptr = buf->buf;
    if(!buf->buf)
        return -ENOMEM;
    buf->bufend = buf->buf + 1024;
    return 0;
}

void mp_free(struct mp_buf *buf) {
    free(buf->buf);
    buf->buf = buf->ptr = buf->bufend = NULL;
}

static int mp_ensure_bytes(struct mp_buf *buf, int bytes) {
    if(buf->bufend - buf->ptr > bytes)
        return 1;

    long nlen = (buf->bufend - buf->buf)*2;
    char *nbuf = realloc(buf->buf, nlen);
    if(!nbuf)
        return 0;

    buf->ptr = nbuf + (buf->ptr - buf->buf);
    buf->buf = nbuf;
    buf->bufend = nbuf + nlen;
    return 1;
}

static void mp_byte(struct mp_buf *buf, int byte) {
    *(buf->ptr++) = byte;
}

static void mp_u16(struct mp_buf *buf, uint16_t val) {
    *(uint16_t *)buf->ptr = htobe16(val);
    buf->ptr += 2;
}

static void mp_u32(struct mp_buf *buf, uint32_t val) {
    *(uint32_t *)buf->ptr = htobe32(val);
    buf->ptr += 4;
}

static void mp_u64(struct mp_buf *buf, uint64_t val) {
    *(uint64_t *)buf->ptr = htobe64(val);
    buf->ptr += 8;
}

int mp_start_array(struct mp_buf *buf, int len) {
    if(len < 16) {
        if(!mp_ensure_bytes(buf, 1))
            return -ENOMEM;
        mp_byte(buf, 0x90 | len);
    } else if(len < 65536) {
        if(!mp_ensure_bytes(buf, 3))
            return -ENOMEM;
        mp_byte(buf, 0xDC);
        mp_u16(buf, len);
    } else {
        if(!mp_ensure_bytes(buf, 5))
            return -ENOMEM;
        mp_byte(buf, 0xDD);
        mp_u32(buf, len);
    }
    return 0;
}

int mp_start_map(struct mp_buf *buf, int len) {
    if(len < 16) {
        if(!mp_ensure_bytes(buf, 1))
            return -ENOMEM;
        mp_byte(buf, 0x80 | len);
    } else if(len < 65536) {
        if(!mp_ensure_bytes(buf, 3))
            return -ENOMEM;
        mp_byte(buf, 0xDE);
        mp_u16(buf, len);
    } else {
        if(!mp_ensure_bytes(buf, 5))
            return -ENOMEM;
        mp_byte(buf, 0xDF);
        mp_u32(buf, len);
    }
    return 0;
}

int mp_put_int(struct mp_buf *buf, long val) {
    if(val < 0) {
        if(val > -32) {
            if(!mp_ensure_bytes(buf, 1))
                return -ENOMEM;
            mp_byte(buf, (unsigned char)(signed char)val);
        } else if(val > -128) {
            if(!mp_ensure_bytes(buf, 2))
                return -ENOMEM;
            mp_byte(buf, 0xD0);
            mp_byte(buf, (unsigned char)(signed char)val);
        } else if(val > -32768) {
            if(!mp_ensure_bytes(buf, 3))
                return -ENOMEM;
            mp_byte(buf, 0xD1);
            mp_u16(buf, (uint16_t)(int16_t)val);
        } else if(val > -(1L << 31)) {
            if(!mp_ensure_bytes(buf, 5))
                return -ENOMEM;
            mp_byte(buf, 0xD2);
            mp_u32(buf, (uint32_t)(int32_t)val);
        } else {
            if(!mp_ensure_bytes(buf, 5))
                return -ENOMEM;
            mp_byte(buf, 0xD3);
            mp_u64(buf, (uint64_t)(int64_t)val);
        }
    } else {
        if(val < 128) {
            if(!mp_ensure_bytes(buf, 1))
                return -ENOMEM;
            mp_byte(buf, val);
        } else if(val < 65536) {
            if(!mp_ensure_bytes(buf, 3))
                return -ENOMEM;
            mp_byte(buf, 0xCD);
            mp_u16(buf, val);
        } else if(val < (1L << 32)) {
            if(!mp_ensure_bytes(buf, 5))
                return -ENOMEM;
            mp_byte(buf, 0xCE);
            mp_u32(buf, val);
        } else {
            if(!mp_ensure_bytes(buf, 9))
                return -ENOMEM;
            mp_byte(buf, 0xCF);
            mp_u64(buf, val);
        }
    }
    return 0;
}

int mp_put_string(struct mp_buf *buf, const char *val, int len) {
    if(len < 32) {
        if(!mp_ensure_bytes(buf, len+1))
            return -ENOMEM;
        mp_byte(buf, 0xa0 | len);
    } else if(len < 256) {
        if(!mp_ensure_bytes(buf, len+2))
            return -ENOMEM;
        mp_byte(buf, 0xD9);
        mp_byte(buf, len);
    } else if(len < 65536) {
        if(!mp_ensure_bytes(buf, len+3))
            return -ENOMEM;
        mp_byte(buf, 0xDA);
        mp_u16(buf, len);
    } else {
        if(!mp_ensure_bytes(buf, len+5))
            return -ENOMEM;
        mp_byte(buf, 0xDB);
        mp_u32(buf, len);
    }
    memcpy(buf->ptr, val, len);
    buf->ptr += len;
    return 0;
}

void mp_move_to(struct mp_buf *buf, const char **result, int *resultlen) {
    *result = buf->buf;
    *resultlen = (int)(buf->ptr - buf->buf);
    buf->buf = NULL;
    buf->ptr = NULL;
    buf->bufend = NULL;
}
