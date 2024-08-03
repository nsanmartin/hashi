/*
 * The str type is the c string char* type.
 *
 * This name is consistent with standard lib functions
 * such as strcmp, strcpy, strdup, ...
 *
 */

#ifndef _LIP_STR_SIZE_T__
#define _LIP_STR_SIZE_T__

#include <hashi.h>
typedef char* str;

#define KT str
#define KTCmp strp_compare
#define KTCpy strp_copy
#define KTClean(Ptr) do{ if(Ptr) free(*Ptr);}while(0)
#ifndef KHash
#define KHash strp_hash
#endif

#define VT size_t

static inline int strp_compare(str* s1, str* s2) {
    if (*s1 == *s2) { return 0; }
    int null = (-(*s1 == 0) | (*s2 == 0));
    if (null) return null;
    return strcmp(*s1, *s2);
}

static inline int strp_copy(char** dest, char** src) {
    return (*dest = strdup(*src)) == NULL;
}
static inline int strp_hash(char** s) {
    return hashi_djb2_k33(*s, (size_t)strlen(*s));
}
#include <lip.h>

#endif // _LIP_STR_SIZE_T__
