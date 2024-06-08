#include <stdio.h>
#include <rand-word.h>

int fill_2int_word(char* w) {
    size_t wlen = 1 + rand() % (2 * sizeof(int) - 1);
    fill_int_word_rand(w);
    if (wlen > sizeof(int)) { fill_int_word_rand(w + sizeof(int)); }
    w[wlen] = '\n';
    return wlen;
}


int main(int argc, char* argv[]) {
    if (argc == 3) {
        const long N = strtol(argv[1], 0, 0);
        const long SEED = strtol(argv[2], 0, 0);
        srand(SEED);

        char w[1 + 2 * sizeof(int)];
        for (int i = 0; i < N; ++i) {
            size_t l = fill_2int_word(w);
            fwrite(w, 1, l + 1, stdout);
        }
    } else {
        printf("usage %s N SEED\n", argv[0]);
    }
}
