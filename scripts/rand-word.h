#ifndef __RAND_WORD_HASHI_H__
#define __RAND_WORD_HASHI_H__

#include <stdlib.h>

static inline void fill_int_word_rand(char* int_word) {
    int rand_int = rand();
    char* rand_int_str = (char*)&rand_int;
    for (size_t i = 0; i < sizeof(int); ++i) {
        unsigned char c = rand_int_str[i];
        int_word[i] = 'A' + (c % ('Z' - 'A'));
    }
}
#endif
