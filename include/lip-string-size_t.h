/*
 * The string type is the pair size_t (len) char* (c string)
 */
#ifndef _LIP_STRING_SIZE_T__
#define _LIP_STRING_SIZE_T__

#include <hashi.h>
typedef struct { size_t len; char* s; } string;

#define KT string
#define KTCmp stringp_compare
#define KTCpy stringp_copy
#define KTClean string_free
#ifndef KHash
#define KHash stringp_hash
#endif

#define VT size_t

static inline int stringp_compare(string* s1, string* s2) {
    if (s1->s == s2->s) { return 0; }
    if (s1->s == 0 || s2->s == 0 || s1->len != s2->len ) { return -1; }
    for (size_t i = 0; i < s1->len; ++i) {
        if (s1->s[i] != s2->s[i]) { return -1; }
    }
    return 0;
}

static inline int stringp_copy(string* dest, string* src) {
    if (dest->s) { return -1; }
    dest->s = malloc(src->len);
    if (!dest->s) { return -1; }
    memcpy(dest->s, src->s, src->len);
    dest->len = src->len;
    return 0;
}
static inline int stringp_hash(string* s) {
    return hashi_djb2_k33(s->s, s->len);
}

static inline void string_free(string s) {  free(s.s); }
#include <lip.h>

#endif // _LIP_STRING_SIZE_T__
