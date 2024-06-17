#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <hashi.h>
#include <arl.h>
#include <hat.h>
#include <stdio.h>
#include <rand-word.h>
#include <stats.h>

typedef_arl_ptr(char);
typedef_arl(size_t);

typedef_str_hat(size_t);

typedef struct HashTable HashTable;
typedef struct HashTable {
	arl_char_ptr* tab;
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


int fill_table_hat(str_hat_size_t* ht, long n_words) {
    char w[1 + 2 * sizeof(int)];
    while (n_words --> 0) {
        fill_2int_word(w);
        hat_elem_type(ht)*it = 0x0;
        str_hat_at(ht, w, &it);
        if (it) {
            ++it->v;
        } else {
            char* dw = strdup(w);
            if (!dw) { perror("mem error, exiting"); exit(1); }
            str_hat_update(ht, dw, 1);
        }
    }
    return 0;
}
int fill_table(HashTable* ht, long n_words) {
    char w[1 + 2 * sizeof(int)];
    while (n_words --> 0) {
        fill_2int_word(w);

        size_t h = ht->hash(ht, w);
        size_t len = arl_len(ht->tab + h);
        if (len) {
            arl_item_type(ht->tab)* it = arl_find_str(ht->tab + h, w); 
            if (!it) {
                char* copy = strdup(w);
                arl_append(ht->tab + h, copy);
            }
        } else {
            char* copy = strdup(w);
            arl_append(ht->tab + h, copy);
        }
    }
    return 0;
}

int free_words (str_hat_size_t* H) { for (arl_iter_type(&hat_slots(H)) it = arl_iter(&hat_slots(H))
        ; it
        ; it = arl_iter_next(&hat_slots(H), it)
    ) {
        for (arl_iter_type(it) itt = arl_iter(it)
            ; itt
            ; itt = arl_iter_next(it, itt)
        ) {
            free(itt->k);
        }
        arl_cleanup(it);
    }
    return 0;
}


int run_hat(size_t initial_capacity, size_t n_words) {
    str_hat_size_t* H = &str_hat_empty(size_t);
    hat_init(H, initial_capacity);
    if(hat_err(H)) { perror("hat init"); return 1;}
    if (fill_table_hat(H, n_words)) { return 1; }

    for (arl_iter_type(&hat_slots(H)) it = arl_iter(&hat_slots(H))
        ; it
        ; it = arl_iter_next(&hat_slots(H), it)
    ) {
        printf(
            "slot: %ld, len: %ld\n",
            it - arl_iter(&hat_slots(H)),
            arl_len(it)
        );
        for (arl_iter_type(it) itt = arl_iter(it)
            ; itt
            ; itt = arl_iter_next(it, itt)
        ) {
            char* k = itt->k;
            size_t v = itt->v;
            printf("    %s -> %ld\n", k, v);
        }
    }

    free_words(H);
    hat_cleanup(H);
    return 0;
}

int run(size_t initial_capacity, size_t n_words) {
    HashTable ht = (HashTable){ .tab=0x0, .capacity=initial_capacity, .len=0, .hash=ht_hash };
    ht.tab = malloc(ht.capacity * sizeof(*ht.tab));
    if (!ht.tab) { perror("malloc error"); exit(1); }
    for (size_t i = 0; i < ht.capacity; ++i) { ht.tab[i] = (arl_char_ptr){0}; }

    fill_table(&ht, n_words);

    size_t filled = 0;
    arl_size_t* lengths  = &arl_empty(size_t);

    for (size_t i = 0; i < ht.capacity; ++i) {
        size_t len = arl_len(ht.tab + i);
        arl_append(lengths, len);
        if (len) {
            ++filled;
            for (arl_iter_type(ht.tab+i) it= arl_iter(ht.tab+i);
                it;
                it = arl_iter_next(ht.tab+i, it)) {
                free(*it);
            }
            //arl_foreach(ht.tab+i, free);
            arl_cleanup(ht.tab+i);
        }
    }

    Stats st;
    stats_init(&st, arl_items(lengths), arl_len(lengths));
    print_freqs(&st);

    ///
    free(ht.tab);
    arl_cleanup(lengths);
    return 0;
}
int main(int argc, char* argv[]) {
    if (argc == 4) {
        const long N = strtol(argv[1], 0, 0);
        const long CAPACITY = strtol(argv[2], 0, 0);
        const long SEED = strtol(argv[3], 0, 0);
        srand(SEED);

        //run(CAPACITY, N);
        run_hat(CAPACITY, N);
    } else {
        printf("usage %s N CAPACITY SEED\n", argv[0]);
    }
}
