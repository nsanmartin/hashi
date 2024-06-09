#ifndef __HASHI_HAT_H__
#define __HASHI_HAT_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <mua.h>
#include <hashi.h>

enum { HatInitialCapacity = 4 }; // this value is doubled by mua

#define hat_hash_djb2(H, Str, Len) (djb2_k33_len((unsigned char*)Str, Len) % hat_capacity(H))

// Name Macros
/// non ptr k,v
// #define hat_name(Kt, Vt) hat_##Kt##_##Vt
#define hat_name(Kt, Vt) _hashi_cat(hat, _hashi_cat(Kt, Vt))
//#define hat_name_elem(Kt, Vt) hat_##Kt##_##Vt##_elem
#define hat_name_elem(Kt, Vt) _hashi_cat(hat_name(Kt, Vt), elem)
/// ptr key
//#define hat_name_ptrk(Kt, Vt) hat_##Kt##_ptr_##Vt
#define hat_name_ptrk(Kt, Vt) _hashi_cat(hat, _hashi_cat(Kt, _hashi_cat(ptr, Vt)))
//#define hat_name_elem_ptrk(Kt, Vt) hat_##Kt##_ptr_##Vt##_elem
#define hat_name_elem_ptrk(Kt, Vt) _hashi_cat(hat_name_ptrk(Kt, Vt), elem)

#define typedef_hat(Kt, Vt) \
	typedef struct { Kt k; Vt v; } hat_name_elem(Kt, Vt); \
    typedef_mua(hat_name_elem(Kt, Vt)); \
    typedef_mua(mua_name(hat_name_elem(Kt, Vt))); \
	typedef struct { \
        mua_name(mua_name(hat_name_elem(Kt, Vt))) tab; \
        size_t capacity; \
        size_t len; \
        hat_name_elem(Kt, Vt) _elem; \
        Kt _k; \
        Vt _v; \
    } hat_name(Kt, Vt)

#define typedef_hat_ptrk(Kt, Vt) \
	typedef struct { Kt* k; Vt v; } hat_name_elem_ptrk(Kt, Vt); \
	typedef struct { hat_name_elem(Kt, Vt)** tab; size_t capacity; } hat_name_ptrk(Kt, Vt)

#define hat_create(KeyT, ValT) (hat_name(KeyT, ValT)){0}

//    mua_create(typeof(hat_tab(H))); 
#define hat_init(H) do{ \
	hat_capacity(H) = HatInitialCapacity; \
    hat_len(H) = 0; \
    _mua_realloc(&hat_tab(H)); \
    mua_len(&hat_tab(H)) = _mua_cpcty(&hat_tab(H)); \
} while(0);

#define _hat_tab(H) (H)->tab
#define hat_capacity(H) (H)->capacity
#define hat_len(H) (H)->len
#define hat_tab(H) (H)->tab
#define hat_err(H) (mua_err(&hat_tab(H)))
#define hat_elem_type(H) typeof((H)->_elem)

#define hat_ksz(H) sizeof((H)->_k)
#define hat_vsz(H) sizeof((H)->_v)

//TODO: check if present
#define hat_update(H, K, V) do { \
    char __hat_k[sizeof(K)]; \
    typeof(K) __hat_k_lv = K; \
    memcpy(__hat_k, (char*)&__hat_k_lv, sizeof(K)); \
    size_t __hat_h = hat_hash_djb2(H, __hat_k, sizeof(K)); \
    mua_item_type(&_hat_tab(H))* __hat_bkt; \
    __hat_bkt = mua_at(&_hat_tab(H), __hat_h); \
    mua_append(__hat_bkt, ((hat_elem_type(H)){.k=K,.v=V})); \
} while (0)

    //mua_append(mua_at(&_hat_tab(H), __hat_h), ((hat_elem_type(H)){.k=K,.v=V})); 
//    size_t __hat_l = mua_len(mua_at(_hat_tab(H), __hat_h)); 
//    if (__hat_l) { 
//            mua_item_type(ht->tab)* it = mua_find_pointed_zero_terminated(ht->tab + h, w); 

#define hat_at(H, K, OUT) do { \
    char __hat_k[sizeof(K)]; \
    typeof(K) __hat_k_lv = K; \
    memcpy(__hat_k, (char*)&__hat_k_lv, sizeof(K)); \
    size_t __hat_h = hat_hash_djb2(H, __hat_k, sizeof(K)); \
    mua_item_type(&_hat_tab(H))* __hat_bkt; \
    __hat_bkt = mua_at(&_hat_tab(H), __hat_h); \
    mua_item_type(__hat_bkt)* __hat_it = mua_find_substring(__hat_bkt, K, hat_ksz(H)); \
    OUT = __hat_it; \
} while(0)
#endif
