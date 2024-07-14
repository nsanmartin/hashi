//
// Buf (Buffer)
//

#include <ohashi.h>

#ifndef T
#error "Template type T undefined"
#endif

#define buflen(B) (B)->len
typedef struct {
    T* items;
    size_t len;
} BufOf(T);


static inline int
buffn(T, calloc)(BufOf(T)* a, size_t len) {
    if (a->items) { return -1; }
    a->items = calloc(len, sizeof(T)); 
    a->len = len;
    return a->items == 0;
}

static inline int
buffn(T, realloc)(BufOf(T)* a) {
    if (!a->items) { return -1; }
    size_t rest = a->len;
    //TODO: check overflow
    a->len += a->len;
    a->items = realloc(a->items, a->len * sizeof(T)); 
    if (a->items == 0) { return -1; };
    memset(a->items + rest, 0, rest);
    return 0;
} 

static inline T*
buffn(T, at)(BufOf(T)* a, size_t ix) {
    return ix < a->len ? &a->items[ix] : NULL;
}

// static inline T*
// BufFn(T, back)(BufOf(T)* a) {
//     return buf_len(a) ? BufFn(T, at)(a, buf_len(a) - 1) : 0x0;
// }

#ifndef TCpy
static inline int
buffn(T, elem_cpy)(T* dst, const T* src) {
    memmove(dst, src, sizeof(T));
    return 0;
}
#define TCpy  buffn(T, elem_cpy)
#endif // TCpy


//static inline int
//BufFn(T, append)(BufOf(T)* a, const T* ptr) {
//    if (a->len >= a->capacity) {
//        if (BufFn(T, realloc)(a)) { /*error in realloc*/ return  1; }
//    }
//    return TCpy(a->items + a->len++, ptr);
//}

static inline T* buffn(T, iter)(BufOf(T)*a) { return a->items; }
static inline T* buffn(T, end)(BufOf(T)*a) { return a->items + a->len; }

#ifdef TCmp
static inline T*
BufFn(T, find) (BufOf(T)* a, T* x) {
    for (size_t i = 0; i < a->len; ++i) {
        T* addr = &a->items[i];
        if (TCmp(addr, x) == 0) { return addr; }
    }
    return NULL;
}
#endif // TCmp


static inline void
buffn(T, clean)(BufOf(T)*a) {
#ifdef TClean
    for (T* it = buffn(T, iter)(a); it != buffn(T,end)(a); ++it) {
        TClean(it);
    }
#endif
    free(a->items);
}

#undef T
#undef TCmp
#undef TCpy
#undef TClean

