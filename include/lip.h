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
//TODO: make KTClean : KT* -> ()
#if defined(KTClean) || defined(VTClean) 
static inline void
lipfn(KT,VT,clean_entry)(EntryT* e) {
#ifdef KTClean
    KTClean(e->k);
#endif //KTClean
#ifdef VTClean
    VTClean(e->v);
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
    unsigned zerok : 1;
    unsigned char load_factor : 7;
    size_t max_tries;
    size_t inserts;
    BufOf(EntryT) table;
} LipOf(KT,VT);

static inline int
lipfn(KT,VT,init)(LipOf(KT,VT)* l, size_t len) {
    l->max_tries = len;
    l->load_factor = 73;
    return buffn(EntryT, calloc)(liptab(l), len);
}

static inline int
lipfn(KT,VT,is_zero)(KT* k) { return KTCmp(k, &(KT){0}) == 0; }

#ifndef KHash
#define KHash(S) hashi_hash_bytes((char*)S, sizeof(KT))
#endif // KHash

static inline EntryT*
lipfn(KT, VT, find)(LipOf(KT,VT)* l, KT* k, bool* found) {
    size_t h = KHash(k) % l->table.len;
    size_t nmovs = 0;
    while (nmovs++ < l->max_tries) {
        EntryT* e = buffn(EntryT, at)(&l->table, h);
        if (KTCmp(k, &e->k) == 0) { *found = true; return e; }
        if (KTCmp(&e->k, &(KT){0}) == 0) { *found = false; return e; }
        h = (h + 1) % buflen(liptab(l));
    }
    printf(
        "inserts: %ld, size: %ld, max tries: %ld, nmovs: %ld,"
        " load_factor*len/100: %ld\n",
        l->inserts,
        buflen(liptab(l)),
        l->max_tries,
        nmovs,
        l->load_factor* buflen(liptab(l))/100
    ); 
    return 0x0;
}

static inline int lipfn(KT, VT, set)(LipOf(KT,VT)* l, KT* k, VT* v);
static inline VT* lipfn(KT, VT, get)(LipOf(KT,VT)* l, KT* k);

static inline EntryT*
lipfn(KT, VT, __dup_find)(LipOf(KT,VT)* l, KT* k) {
    size_t n = buflen(liptab(l));
    int err = n + n < n;
    if (err) { /* overflow */ return 0x0; }
    //TODO set zero if present

    VT zero_value = (VT){0};
    if (l->zerok) {
        VT* z = lipfn(KT, VT, get)(l, &(KT){0});
        if (!z) { return 0x0; }
        zero_value = *z;
    }

    BufOf(EntryT) t0 = *liptab(l);
    *(liptab(l)) = (BufOf(EntryT)){0};
    lipfn(KT,VT,init)(l, n + n);

    if (l->zerok) {
        if (lipfn(KT,VT,set)(l, &(KT){0}, &zero_value)) {
            return 0x0;
        }
    }

    EntryT* it = buffn(EntryT,iter)(&t0);
    EntryT* end = buffn(EntryT,end)(&t0);
    for(; it != end; ++it) {
        if (!lipfn(KT,VT,is_zero)(&it->k)) {
            bool found;
            EntryT* e = lipfn(KT,VT,find)(l,&it->k,&found);
            if (found) { puts("error"); return 0x0; }
            memmove(e, it, sizeof(EntryT));
        }
    }

    /* We 'move' the pointes/bytes, so no cleanup. */
    free(t0.items);
    bool found;
    EntryT* res = lipfn(KT,VT,find)(l,k,&found);
    if (found) { puts("error"); return 0x0; }
    return res;
}

static inline EntryT*
lipfn(KT, VT, __insert)(LipOf(KT,VT)* l, EntryT* e, KT* k, VT* v) {
        if (l->inserts > buflen(liptab(l)) * l->load_factor / 100) {
            e = lipfn(KT,VT,__dup_find)(l,k);
            if (!e) { return 0x0; }
        }
        if (KTCpy(&e->k, k)) { return 0x0; }
        if (VTCpy(&e->v, v)) {
#ifdef KTClean
            KTClean(e->k);
#endif //KTClean
            return 0x0;
        }
        ++l->inserts;
        return e;
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
        return lipfn(KT,VT,__insert)(l, e, k, v) == NULL;
    }
    return VTCpy(&e->v, v);
}


static inline VT* lipfn(KT,VT,get_or_set)(LipOf(KT,VT)* l, KT* k, VT* v) {
    if (lipfn(KT,VT,is_zero)(k)) { l->zerok = 1; }
    bool found = 0;
    EntryT* e = lipfn(KT,VT,find)(l, k, &found);
    if (!e) { /*TODO ERROR or MAX TRIES?*/
        puts("? max tried?");
        return 0x0; }
    if (!found) {
        if ((e = lipfn(KT,VT,__insert)(l, e, k, v)) == NULL) {
            puts("__insert failed");
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
    if (lipfn(KT,VT,is_zero)(k) && !l->zerok) { return 0x0; }
    bool found = 0;
    EntryT* e = lipfn(KT,VT,find)(l, k, &found);
    if (!e) { /*TODO ERROR or MAX TRIES?*/ return 0x0; }
    if (!found) {
        return 0x0;
    }
    return &e->v;
}


static inline int lipfn(KT, VT, del)(LipOf(KT,VT)* l, KT* k) {
    if (lipfn(KT,VT,is_zero)(k)) {
        if (!l->zerok) { return -1; }
        l->zerok = 0;
        --l->inserts;
        return 0;
    }

    bool found = 0;
    EntryT* e = lipfn(KT,VT,find)(l, k, &found);
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

