#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashi.h>

//TODO: check capacity overflow
#define _mutarr_ensure_more_capacity(Muta)                                          \
    do {                                                                            \
        if (Muta->len >= Muta->capacity) {                                          \
            Muta->capacity += Muta->capacity;                                       \
            Muta->items = realloc(Muta->items, Muta->capacity * Muta->item_sz);     \
            if (!Muta->items) { perror("Realloc failed"); return 1; }               \
        }                                                                           \
    } while(0)

struct Mutarr {
    char* items;
    size_t len;
    size_t capacity;
    size_t item_sz;
};

void* _mutarr_addr_at(Mutarr* a, size_t i) { return (char*)a->items + (a->item_sz * i); }

size_t mutarr_len(Mutarr* a) { return a->len; }
char* mutarr_get(Mutarr* a, size_t i) { return i < a->len ? a->items + (i * a->item_sz) : 0; }
//void* mutarr_get(Mutarr* a, size_t i) { return i < a->len ? (char*)a->items + (i * a->item_sz) : 0; }

int mutarr_at(Mutarr* a, size_t i, char* ptr) {
    if (i >= a->len) { return 1; }
    const void* addr = _mutarr_addr_at(a, i);
    memcpy(ptr, addr, a->item_sz);
    return 0;
}

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


int _mutarr_append_value(Mutarr* a, const char* elem) {
    _mutarr_ensure_more_capacity(a);
    char* addr = _mutarr_addr_at(a, a->len);
    memcpy(addr, &elem, a->item_sz);
    ++a->len;
    return 0;
}

int _mutarr_append_mem(Mutarr* a, char* elem) {
    _mutarr_ensure_more_capacity(a);
    char* addr = _mutarr_addr_at(a, a->len);
    memcpy(addr, elem, a->item_sz);
    ++a->len;
    return 0;
}


void mutarr_foreach(Mutarr* a, int(*fn)(char*)) {
    for (size_t i = 0; i < a->len; ++i) {
        fn(*(char**)mutarr_get(a, i));
    }
}

void mutarr_cleanup(Mutarr* a, void(*free_elem)(void*)) {
    if (a) {
        if (free_elem) {
            for (size_t i = 0; i < a->len; ++i) {
                free_elem(*(char**)mutarr_get(a, i));
            }
        }
        free(a->items);
    }
}


