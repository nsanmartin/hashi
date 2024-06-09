#ifndef __HASHI_MUA_H__
#define __HASHI_MUA_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <macs.h>

/*
 * Every mutable array tpe shall be defined with a typedef_* macro. Thus, typedef_mua(Foo) is transformed to
 *
 * typedef struct {
 *      Foo* items;
 *      size_t len;
 *      size_t capacity;
 * } mua_Foo;
 *
 * But a pointer cannot be passed to this macro, ie.m typedef_mua(int*) won't compile. For that, typedef_mua_prt 
 * shallbe used.
 */
#define typedef_mua(Type) typedef struct { Type* items; size_t len; size_t capacity; } mua_name(Type)
#define typedef_mua_ptr(Type) \
    typedef struct { Type** items; size_t len; size_t capacity; } mua_name_ptr(Type)

#define mua_at(Mua, Ix) ((Ix >= (Mua)->len) ? (0x0) : &(_mua_at(Mua, Ix)))
#define mua_create(Type) (mua_name(Type)){0}
#define mua_ptr_create(Type) (mua_name_ptr(Type)){0}
#define mua_err(Mua) ((Mua)->capacity == 0 && (Mua)->len == 1 )
#define mua_item_type(M) typeof(*mua_items(M))
#define mua_len(Mua) ((Mua)->len)
#define mua_items(M) (M)->items
#define mua_name(Type) _hashi_cat(mua, Type)
#define mua_name_ptr(Type) _hashi_cat(mua_name(Type), ptr)
#define mua_append(Mua, Elem) do { \
                if ((Mua)->len >= (Mua)->capacity) { _mua_realloc(Mua); } \
                if (!mua_err(Mua)) _mua_at(Mua, (Mua)->len++) = Elem; \
        } while(0)

// TOOD
static inline void* _mua_find_impl(char* items, char* x, size_t item_sz, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        char* it = (char*)items + item_sz * i;
        if (strncmp(it, x, item_sz) == 0) { return it; }
    }
    return 0x0;
}

static inline void* _mua_find_pointed_zero_terminated_impl(char** items, char* x, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (strcmp(items[i], x) == 0) { return items + i; }
    }
    return 0x0;
}

#define mua_find_substring(M, X, Slen) _mua_find_impl((char*)mua_items(M), (char*)&X, Slen, mua_len(M))
#define mua_find(M, X) _mua_find_impl((char*)mua_items(M), (char*)&X, _mua_item_sz(M), mua_len(M))
#define mua_find_pointed_zero_terminated(M, X) \
    _mua_find_pointed_zero_terminated_impl((char**)mua_items(M), X,  mua_len(M))

#define mua_cleanup(M) free(mua_items(M))
enum { MuaInitialCapacity = 8 };

#define _mua_cpcty(M) (M)->capacity
#define _mua_item_sz(M) sizeof(*mua_items(M))
#define _mua_set_error(Mua) do { (Mua)->capacity = 0; (Mua)->len = 1; } while(0)
#define _mua_at(Mua, Ix) (Mua)->items[Ix]

#define _mua_realloc(M) \
        do { \
            _mua_cpcty(M) = _mua_cpcty(M) ? 2 * _mua_cpcty(M) : MuaInitialCapacity ; \
            (M)->items = realloc((M)->items, (M)->capacity * sizeof(*(M)->items)); \
            if (!(M)->items) { perror("realloc failed"); _mua_set_error(M); } \
        } while(0)

#define mua_foreach(X, Fn) do{ \
    for (size_t __mua_i = 0; __mua_i < mua_len(X); ++__mua_i) { Fn(_mua_at(X, __mua_i)); } \
} while(0)

#endif
