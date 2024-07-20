//
// Lip (Linear probing hash map)
//

#include <stdbool.h>

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


#ifndef KTCmp
#define KTCmp(S1, S2) hashi_compare_bytes(S1, S2, sizeof(KT))
#endif // KTCmp

#ifndef KTCpy
#define KTCpy(Dst, Src) hashi_copy_bytes(Dst, Src, sizeof(KT))
#endif // KTCpy

#ifndef VTCmp
#define VTCmp(S1, S2) hashi_compare_bytes(S1, S2, sizeof(VT))
#endif // VTCmp

#ifndef VTCpy
#define VTCpy(Dst, Src) hashi_copy_bytes(Dst, Src, sizeof(VT))
#endif // VTCpy


//EntryT LipZeroOf(KT,VT) = (EntryT){0};

typedef struct {
    unsigned zerok : 1;
    unsigned max_tries : 7;
    size_t inserts;
    BufOf(EntryT) table;
} LipOf(KT,VT);

static inline int
lipfn(KT,VT,init)(LipOf(KT,VT)* l, size_t len) {
    l->max_tries = 9;
    return buffn(EntryT, calloc)(liptab(l), len);
}

static inline int
lipfn(KT,VT,is_zero)(KT* k) { return KTCmp(k, &(KT){0}) == 0; }

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

static inline EntryT*
lipfn(KT, VT, find)(LipOf(KT,VT)* l, KT* k, bool* found) {
    size_t h = KHash(*k) % l->table.len;
    unsigned nmovs = 0;
    while (nmovs++ < l->max_tries) {
        EntryT* e = buffn(EntryT, at)(&l->table, h);
        if (KTCmp(k, &e->k) == 0) { *found = true; return e; }
        if (KTCmp(&e->k, &(KT){0}) == 0) { *found = false; return e; }
    }
    return 0x0;
}


static inline int
lipfn(KT, VT, set)(LipOf(KT,VT)* l, KT* k, VT* v) {
    bool found = 0;
    EntryT* e = lipfn(KT,VT,find)(l, k, &found);
    if (!e) { return -1; }

    if (lipfn(KT,VT,is_zero)(k)) {
        found = l->zerok;
        l->zerok = 1;
    }

    if (!found) {
        if (KTCpy(&e->k, k)) { return -1; }
        ++l->inserts;
    }
    return VTCpy(&e->v, v);
}



static inline VT* lipfn(KT, VT, get)(LipOf(KT,VT)* l, KT* k) {
    if (lipfn(KT,VT,is_zero)(k) && !l->zerok) { return 0x0; }
    bool found = 0;
    EntryT* e = lipfn(KT,VT,find)(l, k, &found);
    if (!e) { /*TODO ERROR or MAX TRIES?*/ return 0x0; }
    if (!found) {
        return 0x0;
    }
    return &e->v;
}

static inline void lipfn(KT,VT, clean)(LipOf(KT,VT)*l) {
    buffn(EntryT, clean)(liptab(l));
}
#undef KT
#undef VT
#undef KHash 
#undef EntryT
#undef KTCmp
#undef KTCpy

