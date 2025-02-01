/*
 * Arl (Array list)
 *
 * Arl: T, TCmp, TCpy, TClean -> ArlOf(T)
 * ---
 *  + T: element type
 *  + TCmp: element ptr comparator. Default: hashi_compare_bytes 
 *    (memcmp sizeof T)
 *  + TCpy: element ptr copy. Default: hashi_copy_bytes (memmove sizeof T).
 *  + TClean: element ptr clean. Default: (void).
 *
 * Methods:
 * -------
 *
 * append: ArlOf(T)*, T* -> T*
 * at:     ArlOf(T)*, size_t -> T*
 * back:   ArlOf(T)* -> T*
 * begin:  ArlOf(T)* -> T
 * clean:  ArlOf(T)* -> void
 * end:    ArlOf(T)* -> T
 * find:   ArlOf(T)*, T* -> T*
 *
 */

#include <hashi.h>

#ifndef T
#error "Template type T undefined"
#endif

#define arl_len(A) (A)->len

typedef struct ArlOf(T) {
    T* items;
    size_t len;
    size_t capacity;
} ArlOf(T);


static inline int
ArlFn(T, __realloc)(ArlOf(T)* a) {
    if (a->capacity * 2 < a->capacity) { /* overflow */ return -1; }
    a->capacity = a->capacity ? 2 * a->capacity : ArlDefaultInitialCapacity ;
    a->items = realloc((void*)a->items, a->capacity * sizeof(T)); 
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


static inline T*
ArlFn(T, append)(ArlOf(T)* a, const T* ptr) {
    if (a->len >= a->capacity) {
        if (ArlFn(T, __realloc)(a)) { /*error in realloc*/ return  NULL; }
    }
    if (TCpy(a->items + a->len, ptr)) { return NULL; }
    return a->items + a->len++;
}

static inline T* arlfn(T, begin)(ArlOf(T)*a) { return a->items; }
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
    for (T* it = arlfn(T, begin)(a); it != arlfn(T,end)(a); ++it) {
        TClean(it);
    }
#endif
    free((void*)a->items);
    *a = (ArlOf(T)){0};
}

#undef T
#undef TCmp
#undef TCpy
#undef TClean

