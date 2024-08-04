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
 * at:      BufOf(T)*, size_t -> T*
 * begin:   BufOf(T)* -> T
 * calloc:  BufOf(T)* -> int
 * clean:   BufOf(T)* -> void
 * end:     BufOf(T)* -> T
 * find:    BufOf(T)*, T* -> T*
 * realloc: BufOf(T)* -> int
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
} BufOf(BT);


static inline int
buffn(BT, calloc)(BufOf(BT)* a, size_t len) {
    if (!len || a->items) { return -1; }
    a->items = calloc(len, sizeof(BT)); 
    a->len = len;
    return a->items == 0;
}

static inline BT*
buffn(BT, realloc)(BufOf(BT)* b, size_t len) {
    size_t rest = b->len;
    if (b->len + len < b->len) { /* overflow */ return NULL; }
    b->len += len;
    b->items = realloc(b->items, b->len * sizeof(BT)); 
    if (b->items == 0) { return NULL; };
    return b->items + rest;
} 


static inline int
buffn(BT, append)(BufOf(BT)* b, BT* data, size_t len) {
    if (!data || !len) { return -1; }
    BT* rest = buffn(BT, realloc)(b, len);
    if (!rest) { return -1; }
    memcpy(rest, data, len);
    return 0;
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
buffn(BT, clean)(BufOf(BT)*a) {
#ifdef BTClean
    for (BT* it = buffn(BT, begin)(a); it != buffn(BT,end)(a); ++it) {
        BTClean(it);
    }
#endif
    free(a->items);
}

#undef BT
#undef BTCmp
#undef BTCpy
#undef BTClean

