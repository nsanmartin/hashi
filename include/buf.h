/*
 * Buf (Buffer)
 *
 * Buf: BT, BTCmp, BTCpy, BTClean -> ArlOf(BT)
 * ---
 *  + BT: element type
 *  + BTCmp: element ptr comparator. Default: hashi_compare_bytes
 *    (memcmp sizeof T)
 *  + BTCpy: element ptr copy. Default: hashi_copy_bytes
 *           (memmove sizeof BT).
 *  + BTClean: element ptr clean. Default: (void).
 *
 * Methods:
 * -------
 *
 * append:            BufOf(T)*, T*, size_t  ->  T*
 * at:                BufOf(T)*, size_t  ->  T*
 * begin:             BufOf(T)*  ->  T
 * calloc:            BufOf(T)*  ->  T*
 * clean:             BufOf(T)*  ->  void
 * end:               BufOf(T)*  ->  T
 * find:              BufOf(T)*, T*  ->  T*
 * prepend:           BufOf(T)*, T*, size_t  ->  T*
 * reset:             BufOf(T)*  ->  void
 *
 */

#include <hashi.h>

#ifndef BT
#error "Template type BT undefined"
#endif

#define buflen(B) (B)->len
typedef struct {
    BT* items;
    size_t len;
    size_t capacity;
} BufOf(BT);


static inline BT*
buffn(BT, calloc)(BufOf(BT)* a, size_t len) {
    if (!len || a->items) { return NULL; }
    a->items    = calloc(len, sizeof(BT)); 
    a->len      = len;
    a->capacity = len;
    return a->items ? a->items : NULL;
}

static inline BT*
buffn(BT, __ensure_extra_capacity)(BufOf(BT)* b, size_t len) {
    if (b->len + len < b->len) { /* overflow */ return NULL; }
    size_t rest_offset = b->len;
    if (b->len + len > b->capacity) {
        len -= b->capacity - b->len;
        b->capacity  += len;
        b->items = realloc((void*)b->items, b->capacity * sizeof(BT)); 
        if (!b->items) { return NULL; };
    }

    return b->items + rest_offset;
} 


static inline BT*
buffn(BT, append)(BufOf(BT)* b, BT* data, size_t len) {
    if (!data || !len) { return NULL; }
    BT* rest = buffn(BT, __ensure_extra_capacity)(b, len);
    if (!rest) { return NULL; }
    memcpy((void*)rest, data, len * sizeof(BT));
    b->len += len;
    return rest;
}

static inline BT*
buffn(BT, prepend)(BufOf(BT)* b, BT* data, size_t len) {
    if (!data || !len) { return NULL; }
    BT* rest = buffn(BT, __ensure_extra_capacity)(b, len);
    if (!rest) { return NULL; }
    if (b->len)
        memmove((void*)(b->items + len), b->items, b->len * sizeof(BT));
    memcpy((void*)b->items, data, len * sizeof(BT));
    b->len += len;
    return rest;
}

static inline BT*
buffn(BT, at)(BufOf(BT)* a, size_t ix) {
    return ix < a->len ? &a->items[ix] : NULL;
}

#ifndef BTCmp
#define BTCmp(S1, S2) hashi_compare_bytes(S1, S2, sizeof(BT))
#endif // BTCmp


#ifndef BTCpy
#define BTCpy(Dst, Src) hashi_copy_bytes(Dst, Src, sizeof(BT))
#endif // BTCpy


static inline BT* buffn(BT, begin)(BufOf(BT)*a) { return a->items; }
static inline BT* buffn(BT, end)(BufOf(BT)*a) { return a->items + a->len; }

#ifdef BTCmp
static inline BT*
buffn(BT, find) (BufOf(BT)* a, BT* x) {
    for (size_t i = 0; i < a->len; ++i) {
        BT* addr = &a->items[i];
        if (BTCmp(addr, x) == 0) { return addr; }
    }
    return NULL;
}
#endif // BTCmp


static inline void
buffn(BT, reset)(BufOf(BT)*a) {
#ifdef BTClean
    for (BT* it = buffn(BT, begin)(a); it != buffn(BT,end)(a); ++it) {
        BTClean(it);
    }
#endif
    a->len = 0;
}


static inline void
buffn(BT, clean)(BufOf(BT)*a) {
    buffn(BT, reset)(a);
    free((void*)a->items);
    *a = (BufOf(BT)){0};
}

#undef BT
#undef BTCmp
#undef BTCpy
#undef BTClean

