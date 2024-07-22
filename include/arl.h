//
// Arl (Array list)
//

#include <ohashi.h>

#ifndef T
#error "Template type T undefined"
#endif

#define arl_len(A) (A)->len

typedef struct {
    T* items;
    size_t len;
    size_t capacity;
} ArlOf(T);


static inline int
ArlFn(T, calloc)(ArlOf(T)* a, size_t len) {
    if (a->items) { return -1; }
    a->items = calloc(len, sizeof(T)); 
    a->len = len;
    return a->items == 0;
}

static inline int
ArlFn(T, realloc)(ArlOf(T)* a) {
    if (a->capacity * 2 < a->capacity) { /* overflow */ return -1; }
    a->capacity = a->capacity ? 2 * a->capacity : ArlDefaultInitialCapacity ;
    a->items = realloc(a->items, a->capacity * sizeof(T)); 
    return a->items == 0;
} 

static inline T*
ArlFn(T, at)(ArlOf(T)* a, size_t ix) {
    return ix < a->len ? &a->items[ix] : NULL;
}

static inline T*
ArlFn(T, back)(ArlOf(T)* a) {
    return arl_len(a) ? ArlFn(T, at)(a, arl_len(a) - 1) : 0x0;
}

#ifndef TCmp
#define TCmp(S1, S2) hashi_compare_bytes(S1, S2, sizeof(T))
#endif // TCmp


#ifndef TCpy
#define TCpy(Dst, Src) hashi_copy_bytes(Dst, Src, sizeof(T))
#endif // TCpy


static inline int
ArlFn(T, append)(ArlOf(T)* a, const T* ptr) {
    if (a->len >= a->capacity) {
        if (ArlFn(T, realloc)(a)) { /*error in realloc*/ return  1; }
    }
    return TCpy(a->items + a->len++, ptr);
}

static inline T* arlfn(T, iter)(ArlOf(T)*a) { return a->items; }
static inline T* arlfn(T, end)(ArlOf(T)*a) { return a->items + a->len; }

#ifdef TCmp
static inline T*
ArlFn(T, find) (ArlOf(T)* a, T* x) {
    for (size_t i = 0; i < a->len; ++i) {
        T* addr = &a->items[i];
        if (TCmp(addr, x) == 0) { return addr; }
    }
    return NULL;
}
#endif // TCmp


static inline void
arlfn(T, clean)(ArlOf(T)*a) {
#ifdef TClean
    for (T* it = arlfn(T, iter)(a); it != arlfn(T,end)(a); ++it) {
        TClean(it);
    }
#endif
    free(a->items);
}

#undef T
#undef TCmp
#undef TCpy
#undef TClean

