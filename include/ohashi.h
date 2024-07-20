
#ifndef __HASHI_G_Functions_H__
#define __HASHI_G_Functions_H__

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

// enum { ArlDefaultInitialCapacity = 2 };
#define LipOf(KT, VT) CAT_IND(CAT_IND(lip, KT), VT)
#define lipfn(KT, VT, Name) CAT_IND(LipOf(KT, VT), Name)
#define LipEntryOf(KT, VT) CAT_IND(LipOf(KT, VT), entry)
#define LipZeroOf(KT, VT) CAT_IND(LipOf(KT, VT), zero)

static inline int
hashi_compare_bytes(const void* s1, const void* s2, size_t n) {
    return memcmp(s1, s2, n);
}

static inline int
hashi_copy_bytes(void* dest, const void* src, size_t n) {
    memmove(dest, src, n); return 0;
}
#endif // __HASHI_G_Functions_H__
