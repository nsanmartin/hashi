#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <lip-string-size_t.h>
#include <util.h>

int entry_compare(const void* a, const void* b) {
    const string* sa = a;
    const string* sb = b;

    size_t len = sa->len <= sb->len ? sa->len : sb->len;
    int r = memcmp(sa->s, sb->s, len);
    if (r) { return r; }
    return sa->len - sb->len;
}

int process_line(LipOf(string,size_t)* lip, char* line) {
    size_t zero = 0;
    line = skip_space(line);

    while (*line) {
        char* wend = next_space(line);
        if (wend > line) {
            size_t* v = lipfn(string,size_t,get_or_set)(
                lip,
                &(string){.s=line, .len=wend-line},
                &zero
            );
            if (!v) { puts("Error aborting"); return -1; }
            *v = *v + 1;
            line = skip_space(wend);
        }
    }
    return 0;
}

int fill_table(LipOf(string,size_t)* lip) {
    char* line = 0x0;
    size_t n = 0;
    ssize_t nread = 0;
    while ((nread = getline(&line, &n, stdin)) != -1) {

        if (line && process_line(lip, line)) {
            puts("Error aborting"); free(line); return -1;
        }
    }
    free(line);
    return 0;
}


int run(size_t initial_capacity, bool sort) {
    LipOf(string,size_t)* ht = &(LipOf(string,size_t)){0};
    int err = lipfn(string,size_t,init)(
        ht, (LipInitArgs){.sz=initial_capacity,.attempts=256}
    );
    if(err) { perror("lip init error"); return -1;}

    err = fill_table(ht);
    if (err) { return -1; }
    typeof(lip_buf(ht))* buf = &lip_buf(ht);

    if (sort) {
        qsort(buf->items, buf->len, sizeof(*buf->items), entry_compare);
    }

    LipEntryOf(string,size_t)* it = buffn(LipEntryOf(string,size_t),iter)(buf);
    LipEntryOf(string,size_t)* end = buffn(LipEntryOf(string,size_t),end)(buf);

    for (; it != end; ++it) {
        if (it->k.s) {
            fwrite(it->k.s, 1, it->k.len, stdout);
            printf(" -> %ld\n", it->v);
        }
    }

    lipfn(string,size_t,clean)(ht);
    return 0;
}
int main(int argc, char* argv[]) {
    if (argc > 2) {
        printf("usage %s [INITIAL CAPACITY]\n", argv[0]);
        return 0;
    }

    long CAPACITY = 200;
    if (argc == 2) { CAPACITY = strtol(argv[1], 0, 0); } 
    run(CAPACITY, false);
}

