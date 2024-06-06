#ifndef __HASHI_MUA_H__
#define __HASHI_MUA_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define typedef_mua(Type) typedef struct { Type* items; size_t len; size_t capacity; } mua_name(Type)
#define typedef_mua_ptr(Type) \
	typedef struct { Type** items; size_t len; size_t capacity; } mua_name_ptr(Type)

#define mua_at(Mua, Ix) ((Ix >= (Mua)->len) ? (0x0) : &(_mua_at(Mua, Ix)))
#define mua_create(Type) (mua_name(Type)){0}
#define mua_ptr_create(Type) (mua_name_ptr(Type)){0}
#define mua_err(Mua) ((Mua)->capacity == 0 && (Mua)->len == 1 )
#define mua_item_type(M) typeof(*_mua_items(M))
#define mua_len(Mua) ((Mua)->len)
#define mua_name(Type) mua_##Type
#define mua_name_ptr(Type) mua_##Type##_ptr
#define mua_append(Mua, Elem) do { \
                if ((Mua)->len >= (Mua)->capacity) { _mua_realloc(Mua); } \
                if (!mua_err(Mua)) _mua_at(Mua, (Mua)->len++) = Elem; \
        } while(0)

// TOOD
static inline void* _mua_find_impl(char* items, void* x, size_t item_sz, size_t len) {
	for (size_t i = 0; i < len; ++i) {
		char* it = (char*)items + item_sz * i;
		if (strncmp(it, x, item_sz) == 0) { return it; }
	}
	return 0x0;
}

#define mua_find(M, X) _mua_find_impl((char*)_mua_items(M), (char*)&X, _mua_item_sz(M), mua_len(M))
#define mua_cleanup(M) free(_mua_items(M))
enum { MuaInitialCapacity = 8 };

#define _mua_cpcty(M) (M)->capacity
#define _mua_items(M) (M)->items
#define _mua_item_sz(M) sizeof(*_mua_items(M))
#define _mua_set_error(Mua) do { (Mua)->capacity = 0; (Mua)->len = 1; } while(0)
#define _mua_at(Mua, Ix) (Mua)->items[Ix]

#define _mua_realloc(M) \
        do { \
            if (mua_len(M) >= _mua_cpcty(M)) { \
		    _mua_cpcty(M) = _mua_cpcty(M) ? 2 * _mua_cpcty(M) : MuaInitialCapacity ; \
                    (M)->items = realloc((M)->items, (M)->capacity * sizeof(*(M)->items)); \
                    if (!(M)->items) { perror("realloc failed"); _mua_set_error(M); } \
            } \
        } while(0)

#endif
