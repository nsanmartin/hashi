//
// Buf (Buffer)
//

#include <ohashi.h>

#ifndef BT
#error "Template type BT undefined"
#endif

#define buflen(B) (B)->len
typedef struct {
    BT* items;
    size_t len;
} BufOf(BT);


static inline int
buffn(BT, calloc)(BufOf(BT)* a, size_t len) {
    if (a->items) { return -1; }
    a->items = calloc(len, sizeof(BT)); 
    a->len = len;
    return a->items == 0;
}

static inline int
buffn(BT, realloc)(BufOf(BT)* a) {
    if (!a->items) { return -1; }
    size_t rest = a->len;
    if (a->len + a->len < a->len) { /* overflow */ return -1; }
    a->len += a->len;
    a->items = realloc(a->items, a->len * sizeof(BT)); 
    if (a->items == 0) { return -1; };
    memset(a->items + rest, 0, rest);
    return 0;
} 

static inline BT*
buffn(BT, at)(BufOf(BT)* a, size_t ix) {
    return ix < a->len ? &a->items[ix] : NULL;
}

#ifndef TCpy
static inline int
buffn(BT, elem_cpy)(BT* dst, const BT* src) {
    memmove(dst, src, sizeof(BT));
    return 0;
}
#define TCpy  buffn(BT, elem_cpy)
#endif // TCpy


static inline BT* buffn(BT, iter)(BufOf(BT)*a) { return a->items; }
static inline BT* buffn(BT, end)(BufOf(BT)*a) { return a->items + a->len; }

#ifdef TCmp
static inline BT*
BufFn(BT, find) (BufOf(BT)* a, BT* x) {
    for (size_t i = 0; i < a->len; ++i) {
        BT* addr = &a->items[i];
        if (TCmp(addr, x) == 0) { return addr; }
    }
    return NULL;
}
#endif // TCmp


static inline void
buffn(BT, clean)(BufOf(BT)*a) {
#ifdef BTClean
    for (BT* it = buffn(BT, iter)(a); it != buffn(BT,end)(a); ++it) {
        BTClean(it);
    }
#endif
    free(a->items);
}

#undef BT
#undef BTCmp
#undef BTCpy
#undef BTClean

