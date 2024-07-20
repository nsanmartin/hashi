#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <hashi.h>

typedef char* str;
#define KT str
#define VT size_t
#define KCmp strcmp
#define KCpy strcpy
#include <lip.h>

typedef LipOf(str, size_t) HashTable

#include <rand-word.h>
#include <stats.h>

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
        
        LipentryOf(str,size_t)* e = lipfn(str,size_t,find)(ht, w);
        if (!e) { puts("Error aborting"); exit(1); }
        ++e->v;
    }
    return 0;
}

int run(size_t initial_capacity, size_t n_words) {
    HashTable* ht = &(HashTable){0};
    int err = lipfn(int,int,init)(ht, initial_capacity);
    if(err) { perror("lip init"); return -1;}

    if (fill_table(ht, n_words)) { return -1; }
    
    typeof(lip_buf(ht))* buf = &lip_buf(ht);

    LipEntryOf(str,int)* it = buffn(LipEntryOf(str,int),iter);
    LipEntryOf(str,int)* end = buffn(LipEntryOf(str,int),end);

    for (; it != end; ++it) {
        printf("%s -> %ld\n", it->k, it->v);
    }


    //str_hat_free_keys_cleanup(H);
    //Stats st;
    //stats_init(&st, arl_items(lengths), arl_len(lengths));
    //print_freqs(&st);

    //arl_cleanup(lengths);
    //hat_cleanup(H);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 4) {
        const long N = strtol(argv[1], 0, 0);
        const long CAPACITY = strtol(argv[2], 0, 0);
        const long SEED = strtol(argv[3], 0, 0);
        srand(SEED);

        printf("N: %ld. CAPACITY: %ld, SEED: %ld\n", N, CAPACITY, SEED);
        //run(CAPACITY, N);
        run(CAPACITY, N);
    } else {
        printf("usage %s N CAPACITY SEED\n", argv[0]);
    }
}
