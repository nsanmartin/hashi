
#ifndef __HASHI_G_Functions_H__
#define __HASHI_G_Functions_H__

#include <string.h>
#include <stdlib.h>

#define CAT_LITERAL(A, B) A##_##B
#define CAT_IND(A, B) CAT_LITERAL(A,B)

enum { ArlDefaultInitialCapacity = 2 };
#define ArlOf(T) CAT_IND(arl, T)
#define ArlFn(T, Name) CAT_IND(CAT_IND(arl, T), Name)
#define arlfn ArlFn

#endif
