#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <hashi.h>
#include <hat.h>
#include <arl.h>
#include <stats.h>


#define TabLen 20

typedef_str_hat(size_t);
typedef_arl(size_t);

size_t hash(char* s) { return djb2_k33((unsigned char*)s) % TabLen; }

int puts_each(void* e) { return printf("%lx <= %s\n", hash(e), (const char*)e); }

void print_pair(unsigned char* s, size_t h) { printf("%lx: %s\n", h, s); }

char next_word(char** pp, char** beg) {
    char* p = *pp;
    if (!*p || *p == '\n') { *pp = 0x0; return 0; }
    while (*p && isspace(*p)) { ++p; }
    *beg = p;
    while (*p && !isspace(*p)) { ++p; }
    char tmp = *p;
    *p = '\0';
    *pp = p;
    return tmp;
}

void process_line(char* line, str_hat_size_t* ht) {
    char* p = line;
    while (*p) {
        if (!*p || *p == '\n') { return; }
        while (*p && isspace(*p)) { ++p; }
        char* beg = p;
        while (*p && !isspace(*p)) { ++p; }
        char tmp = *p;
        *p = '\0';

        hat_elem_type(ht)*it = 0x0;
        str_hat_at(ht, beg, &it);
        if (it) {
            ++it->v;
        } else {
            char* dw = strdup(beg);
            if (!dw) { perror("mem error, exiting"); exit(1); }
            str_hat_update(ht, dw, 1);
        }

        *p = tmp;
    }
}


int run(str_hat_size_t* ht) {
    ssize_t nread;
    size_t len = 0;
    char* line = NULL;
    while (-1 != (nread = getline(&line, &len, stdin))) {
        process_line(line, ht);
    }

    arl_size_t* lengths = &arl_empty(size_t);
    for (arl_iter_type(&hat_slots(ht)) it = arl_iter(&hat_slots(ht))
        ; it
        ; it = arl_iter_next(&hat_slots(ht), it)
    ) {
        //printf(
        //    "slot: %ld, len: %ld\n",
        //    it - arl_iter(&hat_slots(ht)),
        //    arl_len(it)
        //);
        arl_append(lengths, arl_len(it));
        if (arl_err(lengths)) { perror("mem err appending"); exit(1); }
        //for (arl_iter_type(it) itt = arl_iter(it)
        //    ; itt
        //    ; itt = arl_iter_next(it, itt)
        //) {
        //    char* k = itt->k;
        //    size_t v = itt->v;
        //    printf("    %s -> %ld\n", k, v);
        //}
    }

    str_hat_free_keys_cleanup(ht);
    Stats st;
    stats_init(&st, arl_items(lengths), arl_len(lengths));
    print_freqs(&st);

    arl_cleanup(lengths);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        const long initial_capacity = strtol(argv[1], 0, 0);
        str_hat_size_t* H = &str_hat_empty(size_t);
        hat_init(H, initial_capacity);
        if(hat_err(H)) { perror("hat init"); return 1;}
        run(H);
    } else {
        printf("usage %s CAPACITY\n", argv[0]);
    }
}
