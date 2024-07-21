#ifndef _LIP_STR_SIZE_T__
#define _LIP_STR_SIZE_T__

#include <ohashi.h>
typedef char* str;

#define KT str
#define KTCmp strp_compare
#define KTCpy strp_copy
#define KHash strp_hash
#define KTClean free

#define VT size_t

static inline int strp_compare(str* s1, str* s2) {
    if (*s1 == *s2) { return 0; }
    return *s1 == 0 || *s2 == 0 || strcmp(*s1, *s2);
}

static inline int strp_copy(char** dest, char** src) {
    return (*dest = strdup(*src)) == NULL;
}
static inline int strp_hash(char** s) {
    return hashi_hash_bytes(*s, (size_t)strlen(*s));
}
#include <lip.h>

#endif // _LIP_STR_SIZE_T__
