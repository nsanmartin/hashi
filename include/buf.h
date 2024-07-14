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
    //TODO: check overflow
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

// static inline BT*
// BufFn(BT, back)(BufOf(BT)* a) {
//     return buf_len(a) ? BufFn(BT, at)(a, buf_len(a) - 1) : 0x0;
// }

#ifndef TCpy
static inline int
buffn(BT, elem_cpy)(BT* dst, const BT* src) {
    memmove(dst, src, sizeof(BT));
    return 0;
}
#define TCpy  buffn(BT, elem_cpy)
#endif // TCpy


//static inline int
//BufFn(BT, append)(BufOf(BT)* a, const BT* ptr) {
//    if (a->len >= a->capacity) {
//        if (BufFn(BT, realloc)(a)) { /*error in realloc*/ return  1; }
//    }
//    return TCpy(a->items + a->len++, ptr);
//}

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
#ifdef TClean
    for (BT* it = buffn(BT, iter)(a); it != buffn(BT,end)(a); ++it) {
        TClean(it);
    }
#endif
    free(a->items);
}

#undef BT
#undef BTCmp
#undef BTCpy
#undef BTClean

