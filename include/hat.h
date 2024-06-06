#ifndef __HASHI_HAT_H__
#define __HASHI_HAT_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum { HatInitialCapacity = 8 };

#define Join(KT, VT) KT##_##VT
#define hat_name(Kt, Vt) hat_##Kt##_##Vt
#define hat_name_elem(Kt, Vt) hat_##Kt##_##Vt##_elem

#define typedef_hat(Kt, Vt) \
	typedef struct { Kt k; Vt v; } hat_name_elem(Kt, Vt); \
	typedef struct { hat_name_elem(Kt, Vt)* tab; size_t capacity; } hat_name(Kt, Vt)

#define hat_create(KeyT, ValT) (hat_name(KeyT, ValT)){0}
#define hat_init(H) do{ \
	(H)->tab = malloc(HatInitialCapacity * sizeof(*_hat_tab(H))); \
	hat_capacity(H) = HatInitialCapacity; \
} while(0);

#define _hat_tab(H) (H)->tab
#define hat_capacity(H) (H)->capacity
#define hat_err(H) ((H)->tab == 0x0 && hat_capacity(H) != 0)
#endif
