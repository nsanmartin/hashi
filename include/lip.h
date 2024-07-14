//
// Lip (Linear probing hash map)
//


#include <ohashi.h>

#ifndef KT
#error "Template type KT undefined"
#endif

#ifndef VT
#error "Template type VT undefined"
#endif

#define lip_len(L) (L)->len


typedef struct { KT k; VT v; } LipEntryOf(KT, VT);

#define EntryT LipEntryOf(KT, VT)
#define BT EntryT
#include <buf.h>

#define BufT BufOf(EntryT)

#define lipsz(L) (L)->table.len
#define liptab(L) &(L)->table

typedef struct {
    unsigned zerok : 1;
    unsigned max_tries : 7;
    size_t inserts;
    BufOf(EntryT) table;
} LipOf(KT,VT);

static inline int
lipfn(KT,VT,init)(LipOf(KT,VT)* l, size_t len) {
    return buffn(EntryT, calloc)(liptab(l), len);
}

static inline EntryT*
lipfn(KT, VT, set)(LipOf(KT,VT)* l, KT* k, VT* v) {
    (void)k;
    (void)v;
    return l->table.items;
}

#ifndef KHash
static inline int
lipfn(KT,VT, khash)(KT* k) {
    unsigned long hash = 5381;
    int c;
    const char* end = (const char*)k + sizeof(KT);

    for (const char* s = (const char*)k; s < end; ++s) {
        c = *s;
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}
#define KHash 
#endif // KHash

static inline EntryT* lipfn(KT, VT, get)(LipOf(KT,VT)* l, KT* k) {
    size_t h = KHash(*k) % l->table.len;
    return buffn(EntryT, at)(&l->table, h);
}

static inline void lipfn(KT,VT, clean)(LipOf(KT,VT)*l) {
    buffn(EntryT, clean)(liptab(l));
}
#undef KT
#undef VT
#undef KHash 
#undef EntryT

