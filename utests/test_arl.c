#include <stdio.h>
#include <utests.h>


#define T int
#include <arl.h>

typedef struct { int x, y; float f; } Foo;
#define T Foo
#include <arl.h>

typedef char* str;
#define T str
#include <arl.h>

typedef Foo* Fooptr;
#define T Fooptr
#include <arl.h>

int arl_not_found(void) {
    ArlOf(int)* x = &(ArlOf(int)){0};

    int v = 1;
    int err = arlfn(int, append)(x, &v);
    utest_assert(!err, clean);

    int* value = arlfn(int, at)(x, 1);
    utest_assert(!value, clean);

    value = arlfn(int, at)(x, 0);
    utest_assert(value, clean);
    utest_assert(*value == v, clean);

    for (int i = 0; i < 100; ++i) {
        err = arlfn(int, append)(x, &i);
        utest_assert(!err, clean);

        value = arlfn(int, at)(x, i + 2);
        utest_assert(!value, clean);
    }

    arlfn(int, clean)(x);
    return 0;
clean:
    arlfn(int, clean)(x);
    return 1;
}

int test_arl_1(void) {
    const size_t test_len = 10000;
    ArlOf(int)* x = &(ArlOf(int)){0};

    for (size_t i = 0; i < test_len; ++i) {
        int v = 3*i;
        int err = arlfn(int, append)(x, &v);
        utest_assert(!err, clean);
    }

    utest_assert(arl_len(x) == test_len, clean);

    for (size_t i = 0; i < test_len; ++i) {
        int* value = arlfn(int, at)(x, i);
        utest_assert(value, clean);
        utest_assert(*value == 3*(int)i, clean);
    }
    
    arlfn(int, clean)(x);
    return 0;
clean:
    arlfn(int, clean)(x);
    return 1;
}

//
//int test_arl_2(void) {
//    arl_Foo* x = &arl_empty(Foo);
//    utest_assert(arl_len(x) == 0);
//    arl_append(x, ((Foo){.x=4,.y=7,.f=0}));
//    utest_assert(!arl_err(x));
//    arl_item_type(x)* f = arl_at(x, 0);
//    utest_assert_clean(f);
//    utest_assert_clean(f->x == 4 && f->y == 7 && f->f == 0);
//
//    utest_finally_and_return(arl_cleanup(x));
//}
//
//int test_arl_string(void) {
//    arl_char_ptr* x = &arl_ptr_empty(char);
//    arl_append(x, "foo");
//    utest_assert(!arl_err(x));
//
//    arl_append(x, "bar");
//    utest_assert_clean(!arl_err(x));
//    arl_item_type(x)* v = arl_at(x, 0);
//    utest_assert_clean(v);
//    utest_assert_str_eq_clean("foo", *v);
//    v = arl_at(x, 1);
//    utest_assert_clean(v);
//    utest_assert_str_eq_clean("bar", *v);
//
//    utest_finally_and_return(arl_cleanup(x));
//}
//
//int test_arl_find_int(void) {
//    arl_int* x = &arl_empty(int);
//    arl_append(x, 13);
//    utest_assert(!arl_err(x));
//    arl_append(x, 9);
//    utest_assert_clean(!arl_err(x));
//
//    
//    utest_arl_assert_find(x, 13);
//    utest_arl_assert_find(x, 9);
//
//    utest_arl_assert_find_not(x, 1);
//    utest_arl_assert_find_not(x, 0);
//
//    utest_finally_and_return(arl_cleanup(x));
//}
//
//int test_arl_find_string(void) {
//    arl_char_ptr* x = &arl_ptr_empty(char);
//    arl_append(x, "foo");
//    utest_assert(!arl_err(x));
//
//    arl_append(x, "bar");
//    utest_assert_clean(!arl_err(x));
//
//    utest_arl_assert_find_str(x, "foo");
//    utest_arl_assert_find_str(x, "bar");
//
//    utest_arl_assert_find_not(x, "1");
//    utest_arl_assert_find_not(x, "");
//
//    utest_finally_and_return(arl_cleanup(x));
//}
//
//int test_arl_it(void) {
//    const int min = -999;
//    const int max = 999;
//
//    arl_int* x = &arl_empty(int);
//    arl_append(x, min);
//    utest_assert(!arl_err(x));
//
//    for (int i = min+ 1; i < max + 1; ++i) {
//        arl_append(x, i);
//        utest_assert_clean(!arl_err(x));
//    }
//
//    arl_iter_type(x) it = arl_iter(x);
//    utest_assert_clean(it);
//    utest_assert_clean(*it == min);
//
//    for (int i = min+1; i < max+1; ++i) {
//        it = arl_iter_next(x, it);
//        utest_assert_clean(it);
//        utest_assert_clean(*it == i);
//    }
//
//
//    int i = min;
//    for(it = arl_iter(x) ; it; ++i) {
//        if (*it != i) {
//            utest_assert_clean(*it == i);
//        }
//        it = arl_iter_next(x, it);
//    }
//    utest_finally_and_return(arl_cleanup(x));
//}
//
//int test_arl_calloc(void) {
//    arl_int* x = &arl_empty(int);
//    arl_init_calloc(x, 100);
//    utest_assert(!arl_err(x));
//
//    for (arl_item_type(x)* it = arl_iter(x); it; it = arl_iter_next(x, it)) {
//        utest_assert_clean(it);
//        utest_assert_clean(*it == 0);
//    }
//
//    for (size_t i = 0; i < 100; ++i) {
//        utest_assert_clean(arl_at(x, i));
//        utest_assert_clean(*arl_at(x, i) == 0);
//    }
//
//    utest_finally_and_return(arl_cleanup(x));
//}
//
//int test_arl_find_str(void) {
//    arl_char_ptr* x = &arl_ptr_empty(char);
//    char* foo0 = strdup("foo");
//    char* foo1 = strdup("foo");
//    char* bar0 = strdup("bar");
//    char* bar1 = strdup("bar");
//    char* one = strdup("1");
//    char* empty = strdup("");
//
//    arl_append(x, foo0);
//    utest_assert(!arl_err(x));
//
//    arl_append(x, bar0);
//    utest_assert_clean(!arl_err(x));
//
//    utest_arl_assert_find_str2(x, foo1);
//    utest_arl_assert_find_str2(x, "foo");
//    utest_arl_assert_find_not2(x, "foobar");
//    utest_arl_assert_find_str2(x, bar1);
//
//    utest_arl_assert_find_not2(x, one);
//
//    utest_arl_assert_find_not2(x, empty);
//
//    utest_finally_and_return((
//        arl_cleanup(x)
//        , free(foo0)
//        , free(foo1)
//        , free(bar0)
//        , free(bar1)
//        , free(one)
//        , free(empty)
//    ));
//}

int main(void) {
    int failures = arl_not_found()
        + test_arl_1()
        //+ test_arl_2()
        //+ test_arl_string()
        //+ test_arl_find_int()
        //+ test_arl_find_string()
        //+ test_arl_it()
        //+ test_arl_find_str()
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		puts("Tests Ok!");
	}
	return failures;
}
