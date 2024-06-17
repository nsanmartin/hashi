#ifndef __HASHI_MUA_H__
#define __HASHI_MUA_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <macs.h>

/*
 * Every mutable array tpe shall be defined with a typedef_* macro. Thus, typedef_arl(T) is transformed to
 *
 * typedef struct {
 *      T* items;
 *      size_t len;
 *      size_t capacity;
 * } arl_T;
 *
 * But a pointer cannot be passed to this macro, ie.m typedef_arl(int*) won't compile. For that, typedef_arl_prt 
 * shallbe used.
 */
#define typedef_arl(Type) typedef struct { Type* items; size_t len; size_t capacity; } arl_of(Type)
#define typedef_arl_ptr(Type) \
    typedef struct { Type** items; size_t len; size_t capacity; } arl_of_ptr(Type)

#define arl_empty(Type) (arl_of(Type)){0}
#define arl_from_array(Array, Sz) (arl_of(Type)){.items=Array, .len=Sz, .capacity=Sz}

#define arl_init_calloc(A, Sz) do{\
    arl_items(A) = calloc(Sz, arl_item_size(A)); \
    if (!arl_items(A)) { _arl_set_error(A); } else { arl_len(A) = arl_capacity(A) = Sz; } \
} while(0)


#define arl_len(A) ((A)->len)
#define arl_items(M) ((M)->items)
#define arl_capacity(M) ((M)->capacity)

#define arl_err(A) (arl_capacity(A) == 0 && arl_len(A) == 1 )
#define arl_at(A, Ix) ((Ix >= arl_len(A)) ? 0x0 : arl_items(A) + Ix)
#define arl_ptr_empty(Type) (arl_of_ptr(Type)){0}
#define arl_item_type(M) typeof(*arl_items(M))
#define arl_iter_type(M) typeof(arl_items(M))
#define arl_item_size(M) sizeof(*arl_items(M))
#define arl_of(Type) _hashi_cat(arl, Type)
#define arl_of_ptr(Type) _hashi_cat(arl_of(Type), ptr)
#define arl_append(A, Elem) do { \
    if (arl_len(A) >= arl_capacity(A)) { _arl_realloc_or_set_err(A); } \
    if (!arl_err(A)) { (A)->items[(A)->len++] = Elem; } \
} while(0)


static inline void* _arl_find_prefix_or_empty_impl(
    char* items, char* x, size_t prefix_len, size_t item_sz, size_t len, size_t capacity
) {
    size_t i = 0;
    char* it;
    if (prefix_len <= item_sz) {
        for (; i < len; ++i) {
            it = items + item_sz * i;
            if (strncmp(it, x, prefix_len) == 0) { return it; }
        }
    }
    return i < capacity ? it : 0x0;
}

static inline void* _arl_find_prefix_impl(char* items, char* x, size_t prefix_len, size_t item_sz, size_t len) {
    if (prefix_len <= item_sz) {
        for (size_t i = 0; i < len; ++i) {
            char* it = items + item_sz * i;
            if (strncmp(it, x, prefix_len) == 0) { return it; }
        }
    }
    return 0x0;
}

static inline void* _arl_find_str_impl(
    char** items, char* x, size_t item_sz, size_t len
) {
    for (size_t i = 0; i < len; ++i) {
        char* addr = ((char*)items) + item_sz * i;
        char* key_str = *(char**)addr;
        if (!key_str) { perror("expecting a key string, found NULL\n"); return 0x0;}
        if (strcmp(key_str, x) == 0) { return addr; }
    }
    return 0x0;
}

#define arl_find_prefix(M, X, PrefixLen) \
    _arl_find_prefix_impl((char*)arl_items(M), (char*)&X, PrefixLen, arl_item_size(M), arl_len(M))

#define arl_find(M, X) \
    _arl_find_impl((char*)arl_items(M), (char*)&X, arl_item_size(M), arl_item_size(M), arl_len(M))

#define arl_find_str(M, X) \
    _arl_find_str_impl((char**)arl_items(M), X, arl_item_size(M), arl_len(M))

#define arl_cleanup(M) free(arl_items(M))

enum { ArlDefaultInitialCapacity = 8 };

#define _arl_set_error(A) do { arl_capacity(A) = 0; arl_len(A) = 1; } while(0)

#define _arl_realloc_or_set_err(M) \
do { \
    arl_capacity(M) = arl_capacity(M) ? 2 * arl_capacity(M) : ArlDefaultInitialCapacity ; \
    (M)->items = realloc(arl_items(M), arl_capacity(M) * arl_item_size(M)); \
    if (!(M)->items) { perror("realloc failed"); _arl_set_error(M); } \
} while(0)

#define arl_iter(A) (arl_len(A) ? arl_items(A) : 0x0)
#define arl_iter_next(A, It) ((arl_items(A) <= It) && (It + 1 < (arl_items(A) + arl_len(A))) ? It + 1 : 0x0)
#endif
