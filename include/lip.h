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

#define T LipEntryOf(KT, VT)
#include <arl.h>

typedef struct {
    unsigned zerok : 1;
    unsigned max_tries : 7;
    size_t inserts;
    LipEntryOf(KT, VT)* table;
} LipOf(KT,VT);

LipEntryOf(KT, VT)*
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

LipEntryOf(KT, VT)*
lipfn(KT, VT, get)(LipOf(KT,VT)* l, KT* k) {
    size_t h = KHash(*k) % l->table.len;
    return arlfn(LipEntryOf(KT, VT), at)(&l->table, h);
}

#undef KT
#undef VT
#undef KHash 

