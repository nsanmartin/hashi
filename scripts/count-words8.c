#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <hashi.h>
#include <mua.h>
#include <stdio.h>
#include <rand-word.h>
#include <stats.h>

typedef_mua_ptr(char);
typedef_mua(size_t);

typedef struct HashTable HashTable;
typedef struct HashTable {
	mua_char_ptr* tab;
    size_t capacity;
    size_t len;
    size_t (*hash)(HashTable*, char*);
} HashTable;

size_t ht_hash(HashTable* ht, char* s) { return djb2_k33((unsigned char*)s) % ht->capacity; }

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

    size_t filled = 0;
    mua_size_t* lengths  = &mua_create(size_t);

    for (size_t i = 0; i < ht.capacity; ++i) {
        size_t len = mua_len(ht.tab + i);
        mua_append(lengths, len);
        if (len) {
            ++filled;
            mua_foreach(ht.tab+i, free);
            mua_cleanup(ht.tab+i);
        }
    }

    Stats st;
    stats_init(&st, mua_items(lengths), mua_len(lengths));
    print_freqs(&st);

    ///
    free(ht.tab);
    mua_cleanup(lengths);
    return 0;
}
int main(int argc, char* argv[]) {
    if (argc == 4) {
        const long N = strtol(argv[1], 0, 0);
        const long CAPACITY = strtol(argv[2], 0, 0);
        const long SEED = strtol(argv[3], 0, 0);
        srand(SEED);

        run(CAPACITY, N);
    } else {
        printf("usage %s N CAPACITY SEED\n", argv[0]);
    }
}
