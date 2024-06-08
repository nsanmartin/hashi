#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <hashi.h>
#include <mua.h>

#define TabLen 20
typedef_mua_ptr(char);

typedef struct HashTable HashTable;
typedef struct HashTable {
	mua_char_ptr* tab;
    size_t capacity;
    size_t len;
    size_t (*hash)(HashTable*, char*);
} HashTable;

size_t hash(char* s) { return djb2_k33((unsigned char*)s) % TabLen; }
size_t ht_hash(HashTable* ht, char* s) { return djb2_k33((unsigned char*)s) % ht->capacity; }

int puts_each(void* e) { return printf("%lx <= %s\n", hash(e), (const char*)e); }

void print_pair(unsigned char* s, size_t h) { printf("%lx: %s\n", h, s); }

char next_word(char** pp, char** beg) {
    char* p = *pp;
    if (!*p || *p == '\n') { return '\0'; }
    while (*p && isspace(*p)) { ++p; }
    *beg = p;
    while (*p && !isspace(*p)) { ++p; }
    char tmp = *p;
    *p = '\0';
    *pp = p;
    return tmp;
}

void process_line(char* line, mua_char_ptr* X, size_t M) {
    (void)M;
    char* p = line;
    while (*p) {
        if (!*p || *p == '\n') { return; }
        while (*p && isspace(*p)) { ++p; }
        char* beg = p;
        while (*p && !isspace(*p)) { ++p; }
        char tmp = *p;
        *p = '\0';


        size_t h = hash(beg);
        size_t len = mua_len(X + h);
        if (len) {
            mua_item_type(X)* it = mua_find_pointed_zero_terminated(X + h, beg); 
            if (!it) {
                char* copy = strdup(beg);
                mua_append(X + h, copy);
            }
        } else {
            char* copy = strdup(beg);
            mua_append(X + h, copy);
        }
        *p = tmp;
    }
}

void ht_process_line(HashTable* ht, char* line) {
    char* p = line;
    while (*p) {
        if (!*p || *p == '\n') { return; }
        while (*p && isspace(*p)) { ++p; }
        char* beg = p;
        while (*p && !isspace(*p)) { ++p; }
        char tmp = *p;
        *p = '\0';


        size_t h = ht->hash(ht, beg);
        size_t len = mua_len(ht->tab + h);
        if (len) {
            mua_item_type(ht->tab)* it = mua_find_pointed_zero_terminated(ht->tab + h, beg); 
            if (!it) {
                char* copy = strdup(beg);
                mua_append(ht->tab + h, copy);
            }
        } else {
            char* copy = strdup(beg);
            mua_append(ht->tab + h, copy);
        }
        *p = tmp;
    }
}

int main(void) {
    HashTable ht = (HashTable){ .tab=0x0, .capacity=TabLen, .len=0, .hash=ht_hash };
    ht.tab = malloc(ht.capacity * sizeof(*ht.tab));
    if (!ht.tab) { perror("malloc error"); exit(1); }
    for (size_t i = 0; i < ht.capacity; ++i) { ht.tab[i] = (mua_char_ptr){0}; }

    ssize_t nread;
    size_t len = 0;
    char* line = NULL;
    while (-1 != (nread = getline(&line, &len, stdin))) {
        ht_process_line(&ht, line);
    }

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
    mean = mean / TabLen;
    printf("sumL: %lld, mean: %f, min: %f, max: %f\n", sum, mean, min, max);
}
