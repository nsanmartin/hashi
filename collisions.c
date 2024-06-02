#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <hashi.h>

#define TabLen 2000

size_t hash(char* s) { return djb2_k33((unsigned char*)s) % TabLen; }

int puts_each(void* e) { return printf("%lx <= %s\n", hash(e), (const char*)e); }

void print_pair(unsigned char* s, size_t h) { printf("%lx: %s\n", h, s); }

void process_line(char* line, Mutarr** X, size_t M) {
    (void)M;
    char* p = line;
    while (*p) {
        if (!*p || *p == '\n') { return; }
        if (isspace(*p)) { ++p; continue; }
        char* beg = p;
        while (*p && !isspace(*p)) {
            ++p;
        }
        char tmp = *p;
        *p = '\0';
        size_t h = hash(beg);
        if (X[h]) {
            size_t len = mutarr_len(X[h]);
            size_t i = 0;
            for (; i < len; ++i) {
                char* w = *(char**)mutarr_get(X[h], i);
                if (strcmp(beg, w) == 0) { break; }
            }
            if (i >= len) {
                char* copy = strdup(beg);
                mutarr_append(X[h], copy);
            }
        } else {
            X[h] = mutarr_create(char*);
            char* copy = strdup(beg);
            mutarr_append(X[h], copy);
        }
        *p = tmp;
    }
}

int main(void) {
    size_t M = TabLen;
    Mutarr* X[TabLen] = {0};

    ssize_t nread;
    size_t len = 0;
    char* line = NULL;
    while (-1 != (nread = getline(&line, &len, stdin))) {
        process_line(line, X, M);
    }

    double mean = 0;
    double min = UINT_MAX;
    double max = 0;
    long long unsigned sum = 0;
    for (size_t i = 0; i < TabLen; ++i) {
        size_t len = X[i] ? mutarr_len(X[i]) : 0;
        sum += len;
        mean += len;
        min = min > len ? len : min;
        max = max < len ? len : max;
        printf("%ld: %ld\n", i, len);
        mutarr_cleanup(X[i], free);
    }
    mean = mean / TabLen;
    printf("sumL: %lld, mean: %f, min: %f, max: %f\n", sum, mean, min, max);
}
