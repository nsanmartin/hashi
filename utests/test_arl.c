#include <stdio.h>
#include <utests.h>

int value_cmp(void* x, void* y) {
    return strcmp(x, y);
}

#define T int
#define TCmp value_cmp
#include <arl.h>

typedef struct { int x, y; float f; } Foo;
#define T Foo
#include <arl.h>

typedef char* str;
int str_copy(str* dst, const str* src) {
    *dst = strdup(*src);
    return *dst == 0x0;
}

int str_compare(str* s, str* t) { return strcmp(*s, *t); }
void strp_free(str* p) { free(*p); }
#define T str
#define TCpy str_copy
#define TCmp str_compare
#define TClean strp_free
#include <arl.h>

typedef Foo* Fooptr;
#define T Fooptr
#include <arl.h>

int arl_not_found(void) {
    int status = 1;
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

    clean_and_ret(status, clean, arlfn(int, clean)(x));
}

int test_arl_1(void) {
    int status = 1;
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
    
    clean_and_ret(status, clean, arlfn(int, clean)(x));
}


int test_arl_2(void) {
    int status = 1;

    ArlOf(Foo)* x = &(ArlOf(Foo)){0};

    utest_assert(x->len == 0, clean);

    int err = arlfn(Foo, append)(x, &(Foo){.x=4,.y=7,.f=0});
    utest_assert(!err, clean);
    Foo* f = arlfn(Foo, at)(x, 0);
    utest_assert(f, clean);
    utest_assert(f->x == 4 && f->y == 7 && f->f == 0, clean);

    clean_and_ret(status, clean, arlfn(Foo, clean)(x));
}

int test_arl_string(void) {
    int status = 1;

    ArlOf(str)* x = &(ArlOf(str)){0};
    str s = "foo";
    int err = arlfn(str, append)(x, &s);
    utest_assert(!err, clean);

    s = "bar";
    err = arlfn(str, append)(x, &s);
    utest_assert(!err, clean);

    str* v = arlfn(str, at)(x, 0);
    utest_assert(v, clean);
    utest_assert(strcmp("foo", *v) == 0, clean);

    v = arlfn(str, at)(x, 1);
    utest_assert(v, clean);
    utest_assert(strcmp("bar", *v) == 0, clean);


    clean_and_ret(status, clean, arlfn(str, clean)(x));
}


int test_arl_find_int(void) {
    int status = 1;

    ArlOf(int)* x = &(ArlOf(int)){0};
    //arl_int* x = &arl_empty(int);
    int v = 13;
    int err = arlfn(int, append)(x, &v);
    //arl_append(x, 13);
    utest_assert(!err, clean);
    v = 9;
    err = arlfn(int, append)(x, &v);
    utest_assert(!err, clean);

    
    v = 13;
    int* ptr = arlfn(int, find)(x, &v);
    utest_assert(ptr, clean);

    v = 9;
    ptr = arlfn(int, find)(x, &v);
    utest_assert(ptr, clean);

    v = 1;
    ptr = arlfn(int, find)(x, &v);
    utest_assert(!ptr, clean);
    
    v = 0;
    ptr = arlfn(int, find)(x, &v);
    utest_assert(!ptr, clean);

    clean_and_ret(status, clean, arlfn(int, clean)(x));
}

int test_arl_find_string(void) {
    int status = 1;
    ArlOf(str)* x = &(ArlOf(str)){0};

    str s = "foo";
    int err = arlfn(str, append)(x, &s);
    utest_assert(!err, clean);

    s = "bar";
    err = arlfn(str, append)(x, &s);
    utest_assert(!err, clean);

    s = "foo";
    str* ptr = arlfn(str, find)(x, &s);
    utest_assert(ptr, clean);

    s = "bar";
    ptr = arlfn(str, find)(x, &s);
    utest_assert(ptr, clean);

    s = "1";
    ptr = arlfn(str, find)(x, &s);
    utest_assert(!ptr, clean);

    s = "";
    ptr = arlfn(str, find)(x, &s);
    utest_assert(!ptr, clean);

    clean_and_ret(status, clean, arlfn(str, clean)(x));
}

int test_arl_it(void) {
    int status = 1;
    const int min = -999;
    const int max = 999;

    ArlOf(int)* x = &(ArlOf(int)){0};
    int err = arlfn(int, append)(x, &min);
    utest_assert(!err, clean);

    for (int i = min+ 1; i < max + 1; ++i) {
        err = arlfn(int, append)(x, &i);
        utest_assert(!err, clean);
    }

    int* it = arlfn(int, begin)(x); utest_assert(it, clean);
    utest_assert(*it == min, clean);

    for (int i = min; i < max+1; ++i, ++it) {
        utest_assert(it, clean);
        utest_assert(*it == i, clean);
    }


    int i = min;
    int* end = arlfn(int, end)(x);
    for(it = arlfn(int, begin(x)) ; it != end; ++i, ++it) {
        utest_assert(*it == i, clean);
    }

    clean_and_ret(status, clean, arlfn(int, clean)(x));
}



int test_arl_find_str(void) {
    int status = 1;

    ArlOf(str)* x = &(ArlOf(str)){0};

    char* foo0 = strdup("foo");
    int err = arlfn(str, append)(x, &foo0);
    free(foo0);
    utest_assert(!err, clean);

    char* bar0 = strdup("bar");
    err = arlfn(str, append)(x, &bar0);
    free(bar0);
    utest_assert(!err, clean);


    char* foo1 = strdup("foo");
    str* ptr = arlfn(str, find)(x, &foo1);
    free(foo1);
    utest_assert(ptr, clean);

    str lit = "foo";
    ptr = arlfn(str, find)(x, &lit);
    utest_assert(ptr, clean);

    lit = "foobar";
    ptr = arlfn(str, find)(x, &lit);
    utest_assert(!ptr, clean);

    char* bar1 = strdup("bar");
    ptr = arlfn(str, find)(x, &bar1);
    free(bar1);
    utest_assert(ptr, clean);

    char* one = strdup("1");
    ptr = arlfn(str, find)(x, &one);
    free(one);
    utest_assert(!ptr, clean);

    char* empty = strdup("");
    ptr = arlfn(str, find)(x, &empty);
    free(empty);
    utest_assert(!ptr, clean);


    clean_and_ret(status, clean, arlfn(str, clean)(x));
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
		printf("%s: Tests Ok!\n", __FILE__);

	}
	return failures;
}
