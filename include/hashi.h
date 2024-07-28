#ifndef __HASHI_H__
#define __HASHI_H__

#include <stddef.h>

size_t djb2_k33(unsigned char* s);
size_t djb2_k33_len(unsigned char* s, size_t len);


#endif
