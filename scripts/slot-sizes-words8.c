#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <lip-str-size_t.h>

#include <rand-word.h>
#include <stats.h>

#define T size_t
#include <arl.h>

int fill_2int_word(char* w) {
    size_t wlen = 1 + rand() % (2 * sizeof(int) - 1);
    fill_int_word_rand(w);
    if (wlen > sizeof(int)) { fill_int_word_rand(w + sizeof(int)); }
    w[wlen] = '\0';
    return wlen;
}

int fill_table(LipOf(str,size_t)* ht, long n_words) {
    size_t zero = 0;
    char w[1 + 2 * sizeof(int)];
    char* wp = w;
    while (n_words --> 0) {
        fill_2int_word(w);
        size_t* v = lipfn(str,size_t,get_or_set)(
            ht,
            &wp,
            &zero
        );
        if (!v) {
            puts("Error aborting"); return -1;
        }
        *v = *v + 1;
    }
    return 0;
}

int run(size_t initial_capacity, size_t n_words) {
    LipOf(str,size_t)* ht = &(LipOf(str,size_t)){0};
    int err = lipfn(str,size_t,init)(ht, initial_capacity);
    if(err) { perror("lip init error"); return -1;}

    if (fill_table(ht, n_words)) {
        lipfn(str,size_t,clean)(ht);
        return -1;
    }
    
    typeof(lip_buf(ht))* buf = &lip_buf(ht);

    LipEntryOf(str,size_t)* it = buffn(LipEntryOf(str,size_t),begin)(buf);
    LipEntryOf(str,size_t)* end = buffn(LipEntryOf(str,size_t),end)(buf);

    ArlOf(size_t) lengths = {0};

    for (; it != end; ++it) {
        if (it->k) {
            int err = arlfn(size_t,append)(&lengths, &it->v);
            if (err) {
                perror("mem err appending");
                arlfn(size_t,clean)(&lengths);
                exit(1);
            }
        }
    }


    Stats st;
    stats_init(&st, lengths.items, lengths.len);
    print_freqs(&st);

    arlfn(size_t,clean)(&lengths);
    lipfn(str,size_t,clean)(ht);

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 4 || argc == 3) {
        const long N = strtol(argv[1], 0, 0);
        const long SEED = strtol(argv[2], 0, 0);
        const long CAPACITY = argc == 3 ? 8 : strtol(argv[3], 0, 0);
        srand(SEED);

        printf(
            "N: %ld, SEED: %ld, [INITIAL_CAPACITY]: %ld\n",
            N, SEED, CAPACITY
        );
        run(CAPACITY, N);
    } else {
        printf("usage %s N CAPACITY SEED\n", argv[0]);
    }
}
