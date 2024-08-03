/*
 * Lip (Linear probing hash map)
 *
 * Lip: KT, VT, KHash, KTCmp, KTCpy -> LipOf(KT,VT)
 * ---
 *
 * Methods:
 * --------
 *
 *  + clean:      LipOf(KV,TV)* -> int 
 *  + del:        LipOf(KV,TV)*, KT* -> int 
 *  + get:        LipOf(KV,TV)*, KT* -> VT*
 *  + get_or_set: LipOf(KV,TV)*, KT*, VT* -> VT*
 *  + init:       LipOf(KV,TV)*, LipInitArgs -> int
 *  + set:        LipOf(KV,TV)*, KT*, VT* -> int
 *
 * */


#include <stdbool.h>

#include <hashi.h>

#ifndef KT
#error "Template type KT undefined"
#endif

#ifndef VT
#error "Template type VT undefined"
#endif


typedef struct { KT k; VT v; } LipEntryOf(KT, VT);
#define EntryT LipEntryOf(KT, VT)
//TODO: make KTClean : KT* -> ()
#if defined(KTClean) || defined(VTClean) 
static inline void
lipfn(KT,VT,clean_entry)(EntryT* e) {
#ifdef KTClean
    KTClean(&e->k);
#endif //KTClean
#ifdef VTClean
    VTClean(&e->v);
#endif //VTClean
}
#define BTClean lipfn(KT,VT,clean_entry)
#endif // KTClean || VTClean

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

typedef struct {
    /* If a type's zero coincides with plain zero we nees to distinguish
     * it from empty slot */
    size_t attempts;
    size_t inserts;
    EntryT* zero;
    BufOf(EntryT) table;
} LipOf(KT,VT);

static inline int
lipfn(KT,VT,init)(LipOf(KT,VT)* l, LipInitArgs p) {
    if (p.sz == 0) { return 0; }
    p.attempts = p.attempts ? p.attempts : LipMaximumAttempts;
    p.sz = p.sz ? p.sz : LipDefaultSize;
    l->attempts = p.sz < p.attempts ? p.sz : p.attempts;
    l->inserts = 0;
    l->zero = NULL;
    return buffn(EntryT, calloc)(liptab(l), p.sz);
}

static inline int
lipfn(KT,VT,is_zero)(KT* k) { return KTCmp(k, &(KT){0}) == 0; }

#ifndef KHash
#define KHash(S) hashi_djb2_k33((char*)S, sizeof(KT))
#endif // KHash

/*
 * return value:
 * + if value is found, `found` is set to true and a ptr returned
 * + if value is not found, `found` is set to false and a ptr to
 *   the entry were it should be returned.
 * + if nmovs >= attempts, found is set to false en NULL is returned
 * + if an unexpected error in at, found is set to true and NULL returned.
 */
static inline EntryT*
lipfn(KT, VT, __find)(LipOf(KT,VT)* l, KT* k, bool* found) {
    size_t h = KHash(k) % l->table.len;
    size_t nmovs = 0;
    while (nmovs++ < l->attempts) {
        EntryT* e = buffn(EntryT, at)(&l->table, h);
        if (!e) { *found = true; return e; }
        if (KTCmp(k, &e->k) == 0) { *found = true; return e; }
        if (l->zero != e && KTCmp(&e->k, &(KT){0}) == 0) {
            *found = false;
            return e;
        }
        h = (h + 1) % buflen(liptab(l));
    }
    *found = false;
    //printf(
    //    "size: %ld,    \tinserts: %ld, max tries: %ld, nmovs: %ld\n",
    //    buflen(liptab(l)),
    //    l->inserts,
    //    l->attempts,
    //    nmovs
    //); 
    return 0x0;
}

static inline int lipfn(KT, VT, set)(LipOf(KT,VT)* l, KT* k, VT* v);
static inline VT* lipfn(KT, VT, get)(LipOf(KT,VT)* l, KT* k);

static inline int
lipfn(KT, VT, __dup)(LipOf(KT,VT)* l) {
    size_t n = buflen(liptab(l));
    size_t nX2 = n + n;
    int err = nX2 < n;
    if (err) { /* overflow */ return -1; }

    VT zero_value = (VT){0};
    if (l->zero) {
        VT* z = lipfn(KT, VT, get)(l, &(KT){0});
        if (!z) { return -1; }
        zero_value = *z;
    }

    BufOf(EntryT) t0 = *liptab(l);
    *(liptab(l)) = (BufOf(EntryT)){0};
    EntryT* zero = l->zero;
    size_t attempts = nX2 < LipMaximumAttempts ? nX2 : LipMaximumAttempts;
    lipfn(KT,VT,init)(l,(LipInitArgs){.sz=nX2,.attempts=attempts});
    l->zero = zero;

    if (l->zero) {
        if (lipfn(KT,VT,set)(l, &(KT){0}, &zero_value)) {
            return -1;
        }
    }

    EntryT* it = buffn(EntryT,begin)(&t0);
    EntryT* end = buffn(EntryT,end)(&t0);
    for(; it != end; ++it) {
        if (!lipfn(KT,VT,is_zero)(&it->k)) {
            bool found;
            EntryT* e = lipfn(KT,VT,__find)(l,&it->k,&found);
            if (found) { puts("error"); return -1; }
            memmove(e, it, sizeof(EntryT));
        }
    }

    /* We 'move' the pointers/bytes, so no cleanup. */
    free(t0.items);
    return 0;
}


static inline int
lipfn(KT, VT, set)(LipOf(KT,VT)* l, KT* k, VT* v) {
    bool found = 0;
    EntryT* e = lipfn(KT,VT,__find)(l, k, &found);
    while (!e && !found) {
        if (lipfn(KT,VT,__dup)(l)) { return -1; }
        e = lipfn(KT,VT,__find)(l, k, &found);
    }
    if (!e) { return -1; }

    if (lipfn(KT,VT,is_zero)(k)) {
        found = l->zero != NULL;
        l->zero = e;
    }

    if (!found) {
        ++l->inserts;
        if (KTCpy(&e->k, k)) {
            puts("KTCpy failed");
            return -1;
        }
    }
    if (VTCpy(&e->v, v)) { 
        puts("VTCpy failed");
        return -1;
    }
    return 0;

}


static inline VT* lipfn(KT,VT,get_or_set)(LipOf(KT,VT)* l, KT* k, VT* v) {
    if (lipfn(KT,VT,is_zero)(k)) {
        if (!l->zero) { return 0x0; }
        return &l->zero->v;
    }
    bool found = 0;
    EntryT* e = lipfn(KT,VT,__find)(l, k, &found);
    while (!e && !found) {
        if (lipfn(KT,VT,__dup)(l)) { return 0x0; }
        e = lipfn(KT,VT,__find)(l, k, &found);
    }
    if (!e) { /*TODO ERROR or MAX TRIES?*/
        puts("Unxpected error.");
        return 0x0;
    }
    if (lipfn(KT,VT,is_zero)(k)) { l->zero = e; }
    if (!found) {
        ++l->inserts;
        if (KTCpy(&e->k, k)) {
            puts("KTCpy failed");
            return 0x0;
        }
        if (VTCpy(&e->v, v)) {
            puts("VTCpy failed");
            return 0x0;
        }
    }
    return &e->v;
}


static inline VT* lipfn(KT, VT, get)(LipOf(KT,VT)* l, KT* k) {
    if (lipfn(KT,VT,is_zero)(k)) {
        if (!l->zero) { return 0x0; }
        return &l->zero->v;
    }
    bool found = 0;
    EntryT* e = lipfn(KT,VT,__find)(l, k, &found);
    if (!e) { /*TODO ERROR or MAX TRIES?*/ return 0x0; }
    if (!found) {
        return 0x0;
    }
    return &e->v;
}


static inline int lipfn(KT, VT, del)(LipOf(KT,VT)* l, KT* k) {
    if (lipfn(KT,VT,is_zero)(k)) {
        if (!l->zero) { return -1; }
        l->zero = NULL;
        --l->inserts;
        return 0;
    }

    bool found = 0;
    EntryT* e = lipfn(KT,VT,__find)(l, k, &found);
    if (!e || !found) { return -1; }
    --l->inserts;
    return KTCpy(&e->k, &(KT){0});
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

