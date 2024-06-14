#include <utests.h>
#include <arl.h>

#define utest_arl_assert_find_str(A, V) do{ \
    arl_item_type(A) __value_ = V; \
    arl_item_type(A)* it = arl_find_prefix(A, __value_, arl_item_size(A)); \
    utest_assert_clean(it); \
    utest_assert_clean(strcmp(*it, V) == 0); \
} while(0)

#define utest_arl_assert_find_str2(A, V) do{ \
    arl_item_type(A)* it = arl_find_str(A, V); \
    utest_assert_clean(it); \
    utest_assert_clean(strcmp(*it, V) == 0); \
} while(0)

#define utest_arl_assert_find(A, V) do{ \
    arl_item_type(A) __value_ = V; \
    arl_item_type(A)* it = arl_find_prefix(A, __value_, arl_item_size(A)); \
    utest_assert_clean(it); \
    utest_assert_clean(*it == V); \
} while(0)

#define utest_arl_assert_find_not(A, V) do{ \
    arl_item_type(A) __value_ = V; \
    arl_item_type(A)* it = arl_find_prefix(x, __value_, arl_item_size(A)); \
    utest_assert_clean(!it); \
} while(0)

#define utest_arl_assert_find_not2(Mua, V) do{ \
    arl_item_type(x)* it = arl_find_str(x, V); \
    utest_assert_clean(!it); \
} while(0)

#define utest_finally_and_return(Clean) do { \
    Clean; return TestOk; \
fail_cleanup: \
    Clean; return TestFail; \
} while(0)

typedef_arl(int);
typedef struct { int x, y; float f; } Foo;
typedef_arl(Foo);
typedef_arl_ptr(char);
typedef_arl_ptr(Foo);

int arl_not_found(void) {
    arl_int* x = &arl_empty(int);

    arl_append(x, 1);
    utest_assert(!arl_err(x));

    arl_item_type(x)* value = arl_at(x, 9);
    utest_assert_clean(!value);

    value = arl_at(x, 0);
    utest_assert_clean (value);
    utest_assert_clean(*value == 1);

    for (size_t i = 0; i < 100; ++i) {
        arl_append(x, i);
        utest_assert(!arl_err(x));

        arl_item_type(x)* value = arl_at(x, 2 + i);
        utest_assert_clean(!value);
    }

    utest_finally_and_return(arl_cleanup(x));
}

int test_arl_1(void) {
    const size_t test_len = 10000;
    arl_int* x = &arl_empty(int);
    for (size_t i = 0; !arl_err(x) && i < test_len; ++i) {
        arl_append(x, 3*i);
        if (i) { utest_assert_clean(x); } else { utest_assert(x); }
    }
    utest_assert_clean(arl_len(x) == test_len);
    for (size_t i = 0; !arl_err(x) && i < test_len; ++i) {
        arl_item_type(x)* value = arl_at(x, i);
        utest_assert_clean(value);
        utest_assert_clean(*value == 3*(int)i);
    }
    
    utest_finally_and_return(arl_cleanup(x));
}


int test_arl_2(void) {
    arl_Foo* x = &arl_empty(Foo);
    utest_assert(arl_len(x) == 0);
    arl_append(x, ((Foo){.x=4,.y=7,.f=0}));
    utest_assert(!arl_err(x));
    arl_item_type(x)* f = arl_at(x, 0);
    utest_assert_clean(f);
    utest_assert_clean(f->x == 4 && f->y == 7 && f->f == 0);

    utest_finally_and_return(arl_cleanup(x));
}

int test_arl_string(void) {
    arl_char_ptr* x = &arl_ptr_empty(char);
    arl_append(x, "foo");
    utest_assert(!arl_err(x));

    arl_append(x, "bar");
    utest_assert_clean(!arl_err(x));
    arl_item_type(x)* v = arl_at(x, 0);
    utest_assert_clean(v);
    utest_assert_str_eq_clean("foo", *v);
    v = arl_at(x, 1);
    utest_assert_clean(v);
    utest_assert_str_eq_clean("bar", *v);

    utest_finally_and_return(arl_cleanup(x));
}

int test_arl_find_int(void) {
    arl_int* x = &arl_empty(int);
    arl_append(x, 13);
    utest_assert(!arl_err(x));
    arl_append(x, 9);
    utest_assert_clean(!arl_err(x));

    
    utest_arl_assert_find(x, 13);
    utest_arl_assert_find(x, 9);

    utest_arl_assert_find_not(x, 1);
    utest_arl_assert_find_not(x, 0);

    utest_finally_and_return(arl_cleanup(x));
}

int test_arl_find_string(void) {
    arl_char_ptr* x = &arl_ptr_empty(char);
    arl_append(x, "foo");
    utest_assert(!arl_err(x));

    arl_append(x, "bar");
    utest_assert_clean(!arl_err(x));

    utest_arl_assert_find_str(x, "foo");
    utest_arl_assert_find_str(x, "bar");

    utest_arl_assert_find_not(x, "1");
    utest_arl_assert_find_not(x, "");

    utest_finally_and_return(arl_cleanup(x));
}

int test_arl_it(void) {
    const int min = -999;
    const int max = 999;

    arl_int* x = &arl_empty(int);
    arl_append(x, min);
    utest_assert(!arl_err(x));

    for (int i = min+ 1; i < max + 1; ++i) {
        arl_append(x, i);
        utest_assert_clean(!arl_err(x));
    }

    arl_iter_type(x) it = arl_iter(x);
    utest_assert_clean(it);
    utest_assert_clean(*it == min);

    for (int i = min+1; i < max+1; ++i) {
        it = arl_iter_next(x, it);
        utest_assert_clean(it);
        utest_assert_clean(*it == i);
    }


    int i = min;
    for(it = arl_iter(x) ; it; ++i) {
        if (*it != i) {
            utest_assert_clean(*it == i);
        }
        it = arl_iter_next(x, it);
    }
    utest_finally_and_return(arl_cleanup(x));
}

int test_arl_calloc(void) {
    arl_int* x = &arl_empty(int);
    arl_init_calloc(x, 100);
    utest_assert(!arl_err(x));

    for (arl_item_type(x)* it = arl_iter(x); it; it = arl_iter_next(x, it)) {
        utest_assert_clean(it);
        utest_assert_clean(*it == 0);
    }

    for (size_t i = 0; i < 100; ++i) {
        utest_assert_clean(arl_at(x, i));
        utest_assert_clean(*arl_at(x, i) == 0);
    }

    utest_finally_and_return(arl_cleanup(x));
}

int test_arl_find_str(void) {
    arl_char_ptr* x = &arl_ptr_empty(char);
    char* foo0 = strdup("foo");
    char* foo1 = strdup("foo");
    char* bar0 = strdup("bar");
    char* bar1 = strdup("bar");
    char* one = strdup("1");
    char* empty = strdup("");

    arl_append(x, foo0);
    utest_assert(!arl_err(x));

    arl_append(x, bar0);
    utest_assert_clean(!arl_err(x));

    utest_arl_assert_find_str2(x, foo1);
    utest_arl_assert_find_str2(x, "foo");
    utest_arl_assert_find_not2(x, "foobar");
    utest_arl_assert_find_str2(x, bar1);

    utest_arl_assert_find_not2(x, one);

    utest_arl_assert_find_not2(x, empty);

    utest_finally_and_return((
        arl_cleanup(x)
        , free(foo0)
        , free(foo1)
        , free(bar0)
        , free(bar1)
        , free(one)
        , free(empty)
    ));
}

int main(void) {
    int failures = arl_not_found()
        + test_arl_1()
        + test_arl_2()
        + test_arl_string()
        + test_arl_find_int()
        + test_arl_find_string()
        + test_arl_it()
        + test_arl_find_str()
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		puts("Tests Ok!");
	}
	return failures;
}
