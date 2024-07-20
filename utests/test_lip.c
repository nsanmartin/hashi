#include <stdio.h>
#include <utests.h>

#define KT int
#define VT int
#include <lip.h>

int test_0(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    x->zerok = 1;
    x->max_tries = 9;

    size_t initsz = 4;
    int err = lipfn(int,int,init)(x, initsz);
    utest_assert(!err, clean);
    utest_assert(lipsz(x) == initsz, clean);
    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}


int test_1(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 4;
    int err = lipfn(int,int,init)(x, initsz);
    utest_assert(!err, clean);

    int k = 1;
    int v = 1000;
    err = lipfn(int,int,set)(x, &k, &v);
    utest_assert(!err, clean);

    int* g = lipfn(int,int,get)(x, &k);
    utest_assert(g, clean);
    utest_assert(*g == v, clean);

    k = 13;
    g = lipfn(int,int,get)(x, &k);
    utest_assert(!g, clean);


    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}


int test_2(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 1000;
    int err = lipfn(int,int,init)(x, initsz);
    utest_assert(!err, clean);

    for (int i = -10; i < 10; ++i) {
        int v = i * 300;
        err = lipfn(int,int,set)(x, &i, &v);
        utest_assert(!err, clean);
    }

    for (int i = -10; i < 10; ++i) {
        int* p = lipfn(int,int,get)(x, &i);
        utest_assert(p, clean);
        utest_assert(*p == i * 300, clean);
    }


    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}


int test_3(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 10;
    int err = lipfn(int,int,init)(x, initsz);
    utest_assert(!err, clean);

    int k = 0;
    int v = 10;
    err = lipfn(int,int,set)(x, &k, &v);
    utest_assert(!err, clean);

    int* g = lipfn(int,int,get)(x, &k);
    utest_assert(g, clean);
    utest_assert(*g == v, clean);

    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}

int main(void) {
    int failures = test_0()
        + test_1()
        + test_2()
        + test_3()
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		puts("Tests Ok!");
	}
	return failures;
}
