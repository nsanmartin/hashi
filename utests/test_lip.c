#include <stdio.h>
#include <utests.h>

#define KT int
#define VT int
#include <lip.h> // LipOf(int,int)

#include <lip-str-size_t.h> // LipOf(str,int)

#define KT str
#define KTCmp strp_compare
#define KTCpy strp_copy
#define VT int
#define KHash strp_hash
#include <lip.h>


int test_0(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};

    size_t initsz = 4;
    int err = lipfn(int,int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);
    utest_assert(lipsz(x) == initsz, clean);
    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}


int test_1(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 4;
    int err = lipfn(int,int,init)(x, (LipInitArgs){.sz=initsz});
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
    int err = lipfn(int,int,init)(x, (LipInitArgs){.sz=initsz});
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
    int err = lipfn(int,int,init)(x, (LipInitArgs){.sz=initsz});
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


int test_4(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 10;
    int err = lipfn(int,int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

    //set 1 <- 10
    int k = 1;
    int v = 10;
    err = lipfn(int,int,set)(x, &k, &v);
    utest_assert(!err, clean);

    //get 1
    int* g = lipfn(int,int,get)(x, &k);
    utest_assert(g, clean);
    utest_assert(*g == v, clean);

    //del 1
    err = lipfn(int,int,del)(x, &k);
    utest_assert(!err, clean);

    //get 1 (not found)
    g = lipfn(int,int,get)(x, &k);
    utest_assert(!g, clean);


    //del 1 (error, undefined)
    err = lipfn(int,int,del)(x, &k);
    utest_assert(err, clean);

    //clean and return
    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}


int test_5(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 10;
    int err = lipfn(int,int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

    //set 0 <- 10
    int k = 0;
    int v = 10;
    err = lipfn(int,int,set)(x, &k, &v);
    utest_assert(!err, clean);

    //get 0
    int* g = lipfn(int,int,get)(x, &k);
    utest_assert(g, clean);
    utest_assert(*g == v, clean);

    //del 0
    err = lipfn(int,int,del)(x, &k);
    utest_assert(!err, clean);

    //get 0 (not found)
    g = lipfn(int,int,get)(x, &k);
    utest_assert(!g, clean);


    //del 0 (error, undefined)
    err = lipfn(int,int,del)(x, &k);
    utest_assert(err, clean);

    //clean and return
    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}


int test_6(void) {
    int status = 1;
    LipOf(str,int)* x = &(LipOf(str,int)){0};
    size_t initsz = 10;
    int err = lipfn(str,int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

    //set "Foo" <- 11
    char* k = "Foo";
    int v = 11;
    err = lipfn(str,int,set)(x, &k, &v);
    utest_assert(!err, clean);

    //get "Foo"
    int* g = lipfn(str,int,get)(x, &k);
    utest_assert(g, clean);
    utest_assert(*g == v, clean);

    //set "Bar" <- 22
    k = strdup("Bar");
    utest_assert(k, clean);
    v = 22;
    err = lipfn(str,int,set)(x, &k, &v);
    utest_assert(!err, clean);

    //get "Bar"
    g = lipfn(str,int,get)(x, &k);
    utest_assert(g, clean);
    utest_assert(*g == v, clean);
    free(k);

    //get "Fubar"
    k = "Fubar";
    g = lipfn(str,int,get)(x, &k);
    utest_assert(!g, clean);

    //clean and return
    clean_and_ret(status, clean, lipfn(str,int,clean)(x));
}


int lip_int_int_insert_many(LipOf(int,int)* l, int* ks, int* vs, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        int k = ks[i];
        int v = vs[i];
        if (lipfn(int,int,set)(l, &k, &v)) { return -1; }
    }
    return 0;
}


int lip_int_int_get_many(LipOf(int,int)* l, int* ks, int* vs, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        int k = ks[i];
        int v = vs[i];
        int* g = lipfn(int,int,get)(l, &k);
        if (!g || *g != v) { return -1; }
    }
    return 0;
}

int test_7(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 4;
    int err = lipfn(int,int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

    int ks[] = {1, 2, 3, 4, 5, 6};
    int vs[] = {-1, -1, -1, -1, -1, -1 };

    err = lip_int_int_insert_many(x, ks, vs, sizeof(ks)/sizeof(*ks));
    utest_assert(!err, clean);

    err = lip_int_int_get_many(x, ks, vs, sizeof(ks)/sizeof(*ks));
    utest_assert(!err, clean);

    int k = 64;
    int* g = lipfn(int,int,get)(x, &k);
    utest_assert(!g, clean);


    //clean and return
    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}


int test_8(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 4;
    int err = lipfn(int,int,init)(x, (LipInitArgs){.sz=initsz});
    utest_assert(!err, clean);

    int ks[] = {0, 2, 3, 4, 5, 6};
    int vs[] = {-1, -1, -1, -1, -1, -1 };

    err = lip_int_int_insert_many(x, ks, vs, sizeof(ks)/sizeof(*ks));
    utest_assert(!err, clean);

    err = lip_int_int_get_many(x, ks, vs, sizeof(ks)/sizeof(*ks));
    utest_assert(!err, clean);

    int k = 64;
    int* g = lipfn(int,int,get)(x, &k);
    utest_assert(!g, clean);


    //clean and return
    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}

int main(void) {
    int failures 
        = test_0()
        + test_1()
        + test_2()
        + test_3()
        + test_4()
        + test_5()
        + test_6()
        + test_7()
        + test_8()
        ;

	if (failures) {
		fprintf(
            stderr,
            "%d test%s failed\n",
            failures,
            (failures == 1 ? "" : "s")
        );
	} else {
		printf("%s: Tests Ok!\n", __FILE__);
	}
	return failures;
}
