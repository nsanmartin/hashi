#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashi.h>

struct Mutarr {
    void* items;
    size_t len;
    size_t capacity;
    size_t item_sz;
};


size_t mutarr_len(Mutarr* a) { return a->len; }
void* mutarr_at(Mutarr* a, size_t i) { return i < a->len ? (char*)a->items + (i * a->item_sz) : 0; }

int mutarr_initialize(Mutarr* a, size_t item_sz, size_t capacity) {
    if (!(a->items = malloc(item_sz * capacity))) { return 1; }
    a->capacity = capacity;
    a->item_sz = item_sz;
    a->len = 0;
    return 0;
}

Mutarr* mutarr_create_with_capacity(size_t item_sz, size_t capacity) {
    Mutarr* rv = malloc(sizeof(Mutarr));
    if (!rv) { return 0x0; }
    if (mutarr_initialize(rv, item_sz, capacity)) {
        free(rv);
        return 0x0;
    }
    return rv;

}


int mutarr_append(Mutarr* a, void* elem) {
    if (a->len >= a->capacity) {
        size_t new_capacity = a->capacity + a->capacity;
        a->capacity = new_capacity;
        a->items = realloc(a->items, new_capacity * a->item_sz);
        if (!a->items) {
            perror("Realloc failed");
            return 1;
        }
    }
    void* addr = (char*)a->items + (a->item_sz * a->len);
    memcpy(addr, elem, a->item_sz);
    ++a->len;
    return 0;
}


void mutarr_foreach(Mutarr* a, int(*fn)(void*)) {
    for (size_t i = 0; i < a->len; ++i) {
        fn(*(char**)mutarr_at(a, i));
    }
}

void mutarr_cleanup(Mutarr* a, void(*free_elem)(void*)) {
    if (a) {
        if (free_elem) {
            for (size_t i = 0; i < a->len; ++i) {
                free_elem(*(void**)mutarr_at(a, i));
            }
        }
        free(a->items);
    }
}

