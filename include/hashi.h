#ifndef __HASHI_H__
#define __HASHI_H__

#include <stddef.h>

size_t djb2_k33(unsigned char* s);


typedef struct Mutarr Mutarr;

#define mutarr_create(ElemType) mutarr_create_with_capacity(sizeof(ElemType), 8);
struct Mutarr* mutarr_create_with_capacity(size_t item_sz, size_t capacity);
int mutarr_append(Mutarr* a, void* elem);
void mutarr_foreach(Mutarr* a, int(*fn)(void*));
void mutarr_cleanup(Mutarr* a, void(*free_elem)(void*));
size_t mutarr_len(Mutarr* a);
void* mutarr_at(Mutarr* a, size_t i);

#endif
