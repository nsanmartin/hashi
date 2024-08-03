hashi (generic hash table implementation).

`hashi` includes generic hash table and array list types. It uses the
generic programming technique of [clt](https://github.com/glouw/ctl)
(the type-safe template-like c library).

It defines templates that are used to define concrete types. For example,
an `arl` (array list) type of `int` with default comparator, copy and clean
functions is defined including `<arl.h>` after defining `T`:

```C
#define T int
#include <arl.h>
```

And instance is defined by means of the constructor `ArlOf`:

```C
ArlOf(int) a = (ArlOf(int)){0};
```

Or if you prefer
```C
ArlOf(int)* p = &(ArlOf(int)){0};
```

For example, elements can be appended with `append` or accessed with `at`,
but the functions themself are return values of macro-functions.

```C
int i = 2;
arlfn(int, append)(&a, &i); //or arlfn(int, append)(p, &i);

int* a0 = arlfn(int,at)(&a,0);
if (a0) *a0 = 3;
```

The macro `arlfn` receives a type `T` and a name `N` and returns the
function for `N` for an `ArlOf(T)`.

# Types
## Lip (linear probing).

`lip` is the hash table. Its constructor is `LipOf` and receives the key
and value types (and optionally a key type hash function, comparator,
copier and cleaner).


`Lip: KT, VT, KHash, KTCmp, KTCpy -> LipOf(KT,VT)`

`KT`:    key type
`VT`:    value type
`KHash`: hash function (`KT* -> size_t`).
`KTCmp`: comparison function for values of the key type.
`KTCpy`: copy function for values of the key type.


Methods:
--------

+ `clean`:      `LipOf(KV,TV)* -> int`
+ `del`:        `LipOf(KV,TV)*, KT* -> int`
+ `get`:        `LipOf(KV,TV)*, KT* -> VT*`
+ `get_or_set`: `LipOf(KV,TV)*, KT*, VT* -> VT*`
+ `init`:       `LipOf(KV,TV)*, LipInitArgs -> int`
+ `set`:        `LipOf(KV,TV)*, KT*, VT* -> int`


## Arl (Array list)

Arl: T, TCmp, TCpy, TClean -> ArlOf(T)

+ `T`:      element type
+ `TCmp`:   element ptr comparator. Default: `hashi_compare_bytes` (`memcmp
  sizeof T`)
+ `TCpy`:   element ptr copy. Default: `hashi_copy_bytes` (`memmove sizeof
  T`).
+ `TClean`: element ptr clean. Default: `(void)`.

Methods:
-------

+ `append`: `ArlOf(T)*, T* -> int`
+ `at`:     `ArlOf(T)*, size_t -> T*`
+ `back`:   `ArlOf(T)* -> T*`
+ `begin`:  `ArlOf(T)* -> T`
+ `clean`:  `ArlOf(T)* -> void`
+ `end`:    `ArlOf(T)* -> T`
+ `find`:   `ArlOf(T)*, T* -> T*`

`ArlOf(T)` can be iterated using `begin` and `end` like in:

```C
for(it = arlfn(int, begin(x)) ; it != arlfn(int, end(x)); ++it) {
    // ...
}
```

# Usage

To define a `LipOf(KT, VT)` we use macros like:


```C
#define KT int
#define VT int
#include <lip.h> // LipOf(int,int)
```

The key type default comparator is `memcmp` passing a pointer to a value
of key type and `sizeof(KT)`.

The key type default copier is `memmove` passing a pointer to a value
of key type and `sizeof(KT)`.

The key type default clean does nothing.

To define custom key comparator and copy use `KTCmp` and `KTCpy` like:

```C
#define KT str
#define KTCmp strp_compare
#define KTCpy strp_copy
#define VT int
#define KHash strp_hash
#include <lip.h> // LipOf(str, int)
```


`lip-str-size_t.h` defines a `LipOf(char*, size_t)` setting `KTCmp`,
`KTCpy`, `KTClean` and `KHash`.

The `typedef char* str` is needed because the macros used by hashi need
their type parameters to be source code strings instead of actual types.

`lip-string-size_t.h` define a `LipOf(string, size_t)` where `string` is
`typedef` for `struct {char*; size_t len;}`.

