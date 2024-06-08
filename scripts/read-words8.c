#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <hashi.h>
#include <mua.h>
#include <stdio.h>
#include <rand-word.h>

typedef_mua_ptr(char);

typedef struct HashTable HashTable;
typedef struct HashTable {
	mua_char_ptr* tab;
    size_t capacity;
    size_t len;
    size_t (*hash)(HashTable*, char*);
} HashTable;

// size_t hash(char* s) { return djb2_k33((unsigned char*)s) % TabLen; }
size_t ht_hash(HashTable* ht, char* s) { return djb2_k33((unsigned char*)s) % ht->capacity; }

// int puts_each(void* e) { return printf("%lx <= %s\n", hash(e), (const char*)e); }

void print_pair(unsigned char* s, size_t h) { printf("%lx: %s\n", h, s); }
int fill_2int_word(char* w) {
    size_t wlen = 1 + rand() % (2 * sizeof(int) - 1);
    fill_int_word_rand(w);
    if (wlen > sizeof(int)) { fill_int_word_rand(w + sizeof(int)); }
    w[wlen] = '\0';
    return wlen;
}


int fill_table(HashTable* ht, long n_words) {
    char w[1 + 2 * sizeof(int)];
    while (n_words --> 0) {
        fill_2int_word(w);

        size_t h = ht->hash(ht, w);
        size_t len = mua_len(ht->tab + h);
        if (len) {
            mua_item_type(ht->tab)* it = mua_find_pointed_zero_terminated(ht->tab + h, w); 
            if (!it) {
                char* copy = strdup(w);
                mua_append(ht->tab + h, copy);
            }
        } else {
            char* copy = strdup(w);
            mua_append(ht->tab + h, copy);
        }
    }
    return 0;
}


int run(size_t initial_capacity, size_t n_words) {
    HashTable ht = (HashTable){ .tab=0x0, .capacity=initial_capacity, .len=0, .hash=ht_hash };
    ht.tab = malloc(ht.capacity * sizeof(*ht.tab));
    if (!ht.tab) { perror("malloc error"); exit(1); }
    for (size_t i = 0; i < ht.capacity; ++i) { ht.tab[i] = (mua_char_ptr){0}; }

    fill_table(&ht, n_words);

    double mean = 0;
    double min = UINT_MAX;
    double max = 0;
    long long unsigned sum = 0;
    for (size_t i = 0; i < ht.capacity; ++i) {
        size_t len = mua_len(ht.tab + i);
        sum += len;
        mean += len;
        min = min > len ? len : min;
        max = max < len ? len : max;
        if (len) {
            printf("%ld: %ld\n", i, len);
            mua_foreach(ht.tab+i, free);
            mua_cleanup(ht.tab+i);
        }
    }
    mean = mean / ht.capacity;
    printf("sumL: %lld, mean: %f, min: %f, max: %f\n", sum, mean, min, max);
    return 0;
}
int main(int argc, char* argv[]) {
    if (argc == 3) {
        const long N = strtol(argv[1], 0, 0);
        const long SEED = strtol(argv[2], 0, 0);
        srand(SEED);

        run(20, N);
    } else {
        printf("usage %s N SEED\n", argv[0]);
    }
}
