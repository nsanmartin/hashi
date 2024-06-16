#ifndef __HASHI_HAT_H__
#define __HASHI_HAT_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <arl.h>
#include <hashi.h>

#define hat_hash_djb2(H, Str, Len) (djb2_k33_len((unsigned char*)Str, Len) % hat_capacity(H))

// Name Macros
/// non ptr k,v
#define hat_of(Kt, Vt) _hashi_cat(hat, _hashi_cat(Kt, Vt))
#define hat_elem_of(Kt, Vt) _hashi_cat(hat_of(Kt, Vt), elem)
#define typedef_hat(Kt, Vt) \
	typedef struct { Kt k; Vt v; } hat_elem_of(Kt, Vt); \
    typedef_arl(hat_elem_of(Kt, Vt)); \
    typedef_arl(arl_of(hat_elem_of(Kt, Vt))); \
	typedef struct { \
        arl_of(arl_of(hat_elem_of(Kt, Vt))) slots; \
        size_t size; \
        hat_elem_of(Kt, Vt) _elem; \
        Kt _k; \
        Vt _v; \
    } hat_of(Kt, Vt)


#define hat_empty(KeyT, ValT) (hat_of(KeyT, ValT)){0}

#define hat_init(H, Cpcty) do{ \
    arl_init_calloc(&hat_slots(H), Cpcty); \
} while(0);

#define hat_slots(H) ((H)->slots)
#define hat_capacity(H) arl_len(&hat_slots(H))
#define hat_size(H) ((H)->size)
#define _hat_set_error(H) _arl_set_error(&hat_slots(H))
#define hat_err(H) (arl_err(&hat_slots(H)))
#define hat_elem_type(H) typeof((H)->_elem)

#define hat_slot_type(H) arl_item_type(&hat_slots(H))
#define hat_ksz(H) sizeof((H)->_k)
#define hat_ktype(H) typeof((H)->_k)
#define hat_vsz(H) sizeof((H)->_v)
#define hat_vtype(H) typeof((H)->_v)

#define hat_slot_for(H, Klv, OUT) do{ \
    size_t __hat_h = hat_hash_djb2(H, &Klv, sizeof(Klv)); \
    *OUT = arl_at(&hat_slots(H), __hat_h); \
} while(0)

#define hat_at(H, K, OUT) do { \
    hat_ktype(H) __hat_k = K; \
    hat_slot_type(H)* __hat_slot; \
    hat_slot_for(H, __hat_k, &__hat_slot); \
    arl_item_type(__hat_slot)* __hat_it = arl_find_prefix(__hat_slot, __hat_k, hat_ksz(H)); \
    *OUT = __hat_it; \
} while(0)

#define hat_update(H, K, V) do { \
    hat_ktype(H) __hat_k = K; \
    hat_slot_type(H)* __hat_slot; \
    hat_slot_for(H, __hat_k, &__hat_slot); \
    arl_item_type(__hat_slot)* __hat_it = arl_find_prefix(__hat_slot, __hat_k, hat_ksz(H)); \
    if (__hat_it) { __hat_it->v = V; } \
    else { arl_append(__hat_slot, ((hat_elem_type(H)){.k=K, .v=V})); } \
    if (arl_err(__hat_slot)) { _hat_set_error(H); } \
} while(0)

// TODO #define hat_cleanup(A) arl_cleanup(
//

// str hat (string hash table)
//

/// str key
#define str_hat_of(Vt) _hashi_cat(str_hat, Vt)
#define str_hat_elem_of(Vt) _hashi_cat(str_hat_of(Vt), elem)
#define typedef_str_hat(Vt) \
	typedef struct { char* k; Vt v; } str_hat_elem_of(Vt); \
    typedef_arl(str_hat_elem_of(Vt)); \
    typedef_arl(arl_of(str_hat_elem_of(Vt))); \
	typedef struct { \
        arl_of(arl_of(str_hat_elem_of(Vt))) slots; \
        size_t size; \
        str_hat_elem_of(Vt) _elem; \
        char* _k; \
        Vt _v; \
    } str_hat_of(Vt)

#define str_hat_slot_for(H, Klv, OUT) do{ \
    size_t __hat_h = hat_hash_djb2(H, Klv, strlen(Klv)); \
    *OUT = arl_at(&hat_slots(H), __hat_h); \
} while(0)

#define str_hat_at(H, K, OUT) do { \
    hat_slot_type(H)* __hat_slot; \
    str_hat_slot_for(H, K, &__hat_slot); \
    arl_item_type(__hat_slot)* __hat_it = arl_find_str(__hat_slot, K); \
    *OUT = __hat_it; \
} while(0)

#define str_hat_update(H, K, V) do { \
    hat_slot_type(H)* __hat_slot; \
    str_hat_slot_for(H, K, &__hat_slot); \
    arl_item_type(__hat_slot)* __hat_it = arl_find_str(__hat_slot, K); \
    if (__hat_it) { __hat_it->v = V; } \
    else { arl_append(__hat_slot, ((hat_elem_type(H)){.k=K, .v=V})); } \
    if (arl_err(__hat_slot)) { _hat_set_error(H); } \
} while(0)


#endif
