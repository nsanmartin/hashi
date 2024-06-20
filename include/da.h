#ifndef __HASHI_DA_H__
#define __HASHI_DA_H__

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
//#define typedef_arl(Type) typedef struct { Type* items; size_t len; size_t capacity; } arl_of(Type)
//#define typedef_arl_ptr(Type) 
//    typedef struct { Type** items; size_t len; size_t capacity; } arl_of_ptr(Type)

//#define arl_empty(Type) (arl_of(Type)){0}
//#define arl_from_array(Array, Sz) (arl_of(Type)){.items=Array, .len=Sz, .capacity=Sz}

//#define arl_init_calloc(A, Sz) do{
//    arl_items(A) = calloc(Sz, arl_item_size(A)); 
//    if (!arl_items(A)) { _arl_set_error(A); } else { arl_len(A) = arl_capacity(A) = Sz; } 
//} while(0)

enum { DaDefaultInitialCapacity = 4 };
typedef struct {
     size_t len;
     size_t capacity;
     void (*cpy)(void* src, void* dest, void* extra_params);
} Da;

#define typedef_da(Type) \
    typedef struct { Da a; Type* items; } da_of(Type)

#define da_len(A) ((Da*)(A))->len
#define da_capacity(A) ((Da*)(A))->capacity
//static inline char* da_items(Da* a) { return a->items; }
#define da_items(A) (A)->items
// static inline size_t da_itsz(Da* a) { return a->item_sz; }
#define da_itsz(A) sizeof(*da_items(A))

// #define arl_err(A) (arl_capacity(A) == 0 && arl_len(A) == 1 )
// #define arl_ptr_empty(Type) (arl_of_ptr(Type)){0}
// #define arl_item_type(M) typeof(*arl_items(M))
// #define arl_iter_type(M) typeof(arl_items(M))
// #define arl_item_size(M) sizeof(*arl_items(M))
#define da_of(Type) _hashi_cat(Da, Type)
// #define arl_of_ptr(Type) _hashi_cat(arl_of(Type), ptr)

static inline int
_da_realloc(char** items, size_t capacity[static 1], size_t itsz) {
    *capacity = *capacity ? 2 * *capacity : DaDefaultInitialCapacity;
    *items = realloc(*items, *capacity * itsz);
    if (!*items) { perror("realloc failed"); return 1; }
    return 0;
}

static inline int
_da_at(char** items, size_t len, size_t itsz, size_t ix, void* x) {
    if (ix >= len) { return 1; }
    memcpy(x, *items + ix * itsz, itsz);
    return 0;
}

#define da_at(A, Ix, Ptr) \
    _da_at((char**)&da_items(A), da_len(A), da_itsz(A), Ix, Ptr)

static inline int _da_append(
    char** items,
    size_t len[static 1],
    size_t capacity[static 1],
    size_t itsz,
    void* elem
) { 
    if (*len >= *capacity && _da_realloc(items, capacity, itsz)) {return 1;}
    memcpy(*items + *len * itsz, elem, itsz);
    ++*len;
    return 0;
} 

#define da_append(A, Elem) \
    _da_append( \
    (char**)&da_items(A), &da_len(A), &da_capacity(A), da_itsz(A), &Elem)

static inline void* _da_find_prefix_or_empty_impl(
    char* items,
    char* x,
    size_t prefix_len,
    size_t item_sz,
    size_t len,
    size_t capacity
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

static inline void* _da_find_prefix_impl(
    char* items,
    char* x,
    size_t prefix_len,
    size_t item_sz,
    size_t len
) {
    if (prefix_len <= item_sz) {
        for (size_t i = 0; i < len; ++i) {
            char* it = items + item_sz * i;
            if (strncmp(it, x, prefix_len) == 0) { return it; }
        }
    }
    return 0x0;
}

static inline void* _da_find_str_impl(
    char** items, char* x, size_t item_sz, size_t len
) {
    for (size_t i = 0; i < len; ++i) {
        char** addr = items + item_sz * i;
        char* key_str = *addr;
        if (!key_str) {
            perror("expecting a key string, found NULL\n"); return 0x0;
        }
        if (strcmp(key_str, x) == 0) { return addr; }
    }
    return 0x0;
}

///
///

#define _da_find(A, X) \
    _da_find_prefix_impl( \
            da_items(A), (char*)x, da_itsz(A), da_itsz(A), da_len(A));



#define da_find_str(M, X) \
    _da_find_str_impl((char**)da_items(M), X, da_item_size(M), da_len(M))

#define da_cleanup(M) free(da_items(M))

enum { ArlDefaultInitialCapacity = 8 };

#define _da_set_error(A) do { da_capacity(A) = 0; da_len(A) = 1; } while(0)


#define da_iter(A) (da_len(A) ? da_items(A) : 0x0)
#define da_iter_next(A, It) ((da_items(A) <= It) && (It + 1 < (da_items(A) + da_len(A))) ? It + 1 : 0x0)
#endif
