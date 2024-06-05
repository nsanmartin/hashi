#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define typedef_mua(Type) typedef struct { Type* items; size_t len; size_t capacity; } mua_name(Type)
#define typedef_mua_ptr(Type) \
	typedef struct { Type** items; size_t len; size_t capacity; } mua_name_ptr(Type)

#define mua_at(Mua, Ix, Lval) ((Ix >= (Mua)->len) ? (-1) : ((*Lval)=_mua_at(Mua, Ix), 0))
#define mua_create(Type) (mua_name(Type)){0}
#define mua_ptr_create(Type) (mua_name_ptr(Type)){0}
#define mua_err(Mua) ((Mua)->capacity == 0 && (Mua)->len == 1 )
#define mua_len(Mua) ((Mua)->len)
#define mua_name(Type) mua_##Type
#define mua_name_ptr(Type) mua_##Type##_ptr
#define mua_append(Mua, Elem) do { \
                if ((Mua)->len >= (Mua)->capacity) { _mua_realloc(Mua); } \
                if (!mua_err(Mua)) _mua_at(Mua, (Mua)->len++) = Elem; \
        } while(0)



enum { MuaInitialCapacity = 8 };

#define _mua_set_error(Mua) do { (Mua)->capacity = 0; (Mua)->len = 1; } while(0)
#define _mua_at(Mua, Ix) (Mua)->items[Ix]
#define _mua_realloc(Mua) \
        do { \
            size_t capacity = (Mua)->capacity; \
            if (!capacity) { capacity = MuaInitialCapacity; } \
            if ((Mua)->len >= capacity) { capacity += capacity; } \
            if ((Mua)->capacity < capacity) { \
                    (Mua)->capacity = capacity; \
                    (Mua)->items = realloc((Mua)->items, (Mua)->capacity * sizeof(*(Mua)->items)); \
                    if (!(Mua)->items) { perror("Realloc failed"); _mua_set_error(Mua); } \
            } \
        } while(0)

