#include <stdio.h>
#include <utests.h>

#define KT int
#define VT int
#include <lip.h>


typedef char* str;
#define KT str
static inline int
str_compare(str* s1, str* s2) {
    if (*s1 == *s2) { return 0; }
    return *s1 == 0 || *s2 == 0 || strcmp(*s1, *s2);

}
#define KTCmp str_compare
static inline int
str_copy(char** dest, char** src) {
    *dest = strdup(*src);
    return *dest == NULL;
}
#define KTCpy str_copy
#define VT int
static inline int
hash_length(char** s) {
    char* str = *s;
    size_t n = strlen(str);
    return hashi_hash_bytes(str, n);

}
#define KHash hash_length
#define KTClean free
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


int test_4(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    size_t initsz = 10;
    int err = lipfn(int,int,init)(x, initsz);
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
    int err = lipfn(int,int,init)(x, initsz);
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
    int err = lipfn(str,int,init)(x, initsz);
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


int main(void) {
    int failures 
        = test_0()
        + test_1()
        + test_2()
        + test_3()
        + test_4()
        + test_5()
        + test_6()
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
