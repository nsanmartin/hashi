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


//EntryT LipZeroOf(KT,VT) = (EntryT){0};

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

static inline int
lipfn(KT,VT,is_zero)(KT* k) {
    return memcmp(&(KT){0}, k, sizeof (KT)) == 0;
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

static inline EntryT*
lipfn(KT, VT, find)(LipOf(KT,VT)* l, KT* k, bool* found) {
    size_t h = KHash(*k) % l->table.len;
    unsigned nmovs = 0;
    while (nmovs++ < l->max_tries) {
        EntryT* e = buffn(EntryT, at)(&l->table, h);
        if (memcmp(k, &e->k, sizeof(KT))) { *found = true; return e; }
        if (memcmp(&(KT){0}, k,sizeof(KT))) { *found = false; return e; }
    }
    return 0x0;
}


static inline EntryT*
lipfn(KT, VT, set)(LipOf(KT,VT)* l, KT* k, VT* v) {
    bool found = 0;
    EntryT* e = lipfn(KT,VT,find)(l, k, &found);
    if (!e) { return 0x0; }
    if (!found) {
        //copy kmemcpy
    }
    (void)v;
    ////copy v

    return e;
}



static inline EntryT* lipfn(KT, VT, get)(LipOf(KT,VT)* l, KT* k) {
    size_t h = KHash(*k) % l->table.len;
    if (lipfn(KT,VT,is_zero)(k) && !l->zerok) {
        return 0x0;
    }
    bool found = 0;
    EntryT* e = lipfn(KT,VT,find)(l, k, &found);
    if (!e) { /*TODO NEXPECTED ERROR*/ return 0x0; }
    if (!found) {
        return 0x0;
    }
    return e;

    return buffn(EntryT, at)(&l->table, h);
}

static inline void lipfn(KT,VT, clean)(LipOf(KT,VT)*l) {
    buffn(EntryT, clean)(liptab(l));
}
#undef KT
#undef VT
#undef KHash 
#undef EntryT

