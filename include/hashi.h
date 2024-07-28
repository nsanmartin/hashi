/*
 * Global hashi functions, ie, with larger scope than #define T .. #undef T
 * */
#ifndef __HASHI__H__
#define __HASHI__H__

#include <string.h>
#include <stdlib.h>

#define CAT_LITERAL(A, B) A##_##B
#define CAT_IND(A, B) CAT_LITERAL(A,B)


#define BufOf(T) CAT_IND(buf, T)
#define buffn(T, Name) CAT_IND(CAT_IND(buf, T), Name)

enum { ArlDefaultInitialCapacity = 2 };
#define ArlOf(T) CAT_IND(arl, T)
#define ArlFn(T, Name) CAT_IND(CAT_IND(arl, T), Name)
#define arlfn ArlFn

#define LipOf(KT, VT) CAT_IND(CAT_IND(lip, KT), VT)
#define lipfn(KT, VT, Name) CAT_IND(LipOf(KT, VT), Name)
#define LipEntryOf(KT, VT) CAT_IND(LipOf(KT, VT), entry)
#define LipZeroOf(KT, VT) CAT_IND(LipOf(KT, VT), zero)
#define lip_buf(L) (L)->table

static inline int
hashi_compare_bytes(const void* s1, const void* s2, size_t n) {
    return memcmp(s1, s2, n);
}

static inline int
hashi_copy_bytes(void* dest, const void* src, size_t n) {
    memmove(dest, src, n); return 0;
}

static inline size_t
hashi_hash_bytes(char* s, size_t n) {
    size_t hash = 5381;
    const char* end = s + n;

    for (; s < end; ++s) {
        hash = ((hash << 5) + hash) + *s; /* hash * 33 + c */
    }

    return hash;
}


static inline size_t
hashi_djb2_k33_with_seed(char* s, size_t n, size_t seed) {
    unsigned long hash = seed; //5381;

    for (const char* end = s + n; s < end; ++s) {
        hash = ((hash << 5) + hash) + *s; /* hash * 33 + c */
    }

    return hash;
}

static inline size_t
hashi_djb2_k33(char* s, size_t n) {
    return hashi_djb2_k33_with_seed(s, n, 5381);
}

#endif // __HASHI__H__
