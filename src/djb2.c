#include <hashi.h>

size_t djb2_k33_with_seed(unsigned char* str, size_t seed) {
    unsigned long hash = seed; //5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}


inline size_t djb2_k33(unsigned char* s) { return djb2_k33_with_seed(s, 5381); }
