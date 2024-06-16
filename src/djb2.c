#include <hashi.h>
#include <stdio.h>

size_t djb2_k33_with_seed(unsigned char* str, size_t seed) {
    unsigned long hash = seed; //5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

size_t djb2_k33_len_with_seed(unsigned char* s, size_t len, size_t seed) {
    unsigned long hash = seed; //5381;
    for (size_t i = 0; i < len; ++i) {
        hash = ((hash << 5) + hash) + s[i]; /* hash * 33 + c */
    }

    return hash;
}

inline size_t djb2_k33(unsigned char* s) { return djb2_k33_with_seed(s, 5381); }
inline size_t djb2_k33_len(unsigned char* s, size_t len) { return djb2_k33_len_with_seed(s, len, 5381); }

