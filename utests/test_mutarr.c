#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hashi.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

enum { TestOk = 0, TestFail = 1 };

#define utest_assert(Expr)  \
    if (!(Expr)) { fprintf(stderr, RED "Test failed: %s" RESET "\n", __func__); return TestFail; }

#define utest_assert_str_eq(Expected, String) \
    utest_assert(strcmp(Expected, String) == 0)

typedef struct {
    int x, y, z;
    float f;
} TestFoo;

int test_static_strings_append(void) {
    Mutarr* a = mutarr_create(char**);
    // 0:
    mutarr_append(a, "foo");
    utest_assert(mutarr_len(a) == 1);
    char* r;
    int error = mutarr_at(a, 0, (char*)&r);
    utest_assert(!error);
    utest_assert_str_eq(r , "foo");

    // 1:
    mutarr_append(a, "bar");
    // 2:
    mutarr_append(a, "foobar");
    // 3:
    mutarr_append(a, "foobarba");
    utest_assert(mutarr_len(a) == 4);

    error = mutarr_at(a, 1, (char*)&r);
    utest_assert(!error);
    utest_assert_str_eq(r , "bar");

    error = mutarr_at(a, 3, (char*)&r);
    utest_assert(!error);
    utest_assert_str_eq(r , "foobarba");
    
    error = mutarr_at(a, 2, (char*)&r);
    utest_assert(!error);
    utest_assert_str_eq(r , "foobar");
    
    return TestOk;
}

int test_static_strings_at(void) {
    char* static_string = "foo";
    Mutarr* a = mutarr_create(char**);
    mutarr_append(a, static_string);
    char** string_pointer = (char**)mutarr_get(a, 0);
    char* string = *string_pointer;
    utest_assert(static_string == string);
    utest_assert_str_eq(string, static_string);
    utest_assert_str_eq("foo", static_string);
    utest_assert_str_eq("foo", string);
    utest_assert_str_eq("foo", *(char**)mutarr_get(a, 0));
    return TestOk;
}

int test_long(void) {
    Mutarr* a = mutarr_create(long);
    long r;
    int error;
    long value;

    value = 7;
    mutarr_append(a, (char*)value);
    error = mutarr_at(a, 0, (char*)&r);
    utest_assert(!error);
    utest_assert(r == value);
    utest_assert(r == 7);

    value = 4;
    mutarr_append(a, (char*)value);
    error = mutarr_at(a, 1, (char*)&r);
    utest_assert(!error);
    utest_assert(r == value);
    utest_assert(r == 4);

    value = 4321;
    mutarr_append(a, (char*)value);
    error = mutarr_at(a, 2, (char*)&r);
    utest_assert(!error);
    utest_assert(r == value);
    utest_assert(r == 4321);

    error = mutarr_at(a, 20, (char*)&r);
    utest_assert(error);
    mutarr_append(a, (char*)value);

    return TestOk;
}


int test_char(void) {
    Mutarr* a = mutarr_create(char);
    char r;
    int error;

    mutarr_append(a, '\n');
    error = mutarr_at(a, 0, &r);
    utest_assert(!error);
    utest_assert(r == '\n');

    mutarr_append(a, 'C');
    error = mutarr_at(a, 1, &r);
    utest_assert(!error);
    utest_assert(r == 'C');

    char value = '"';
    mutarr_append(a, (long)value);
    error = mutarr_at(a, 2, &r);
    utest_assert(!error);
    utest_assert(r == value);
    utest_assert(r == '"');

    return TestOk;
}

int test_test_foo(void) {
    Mutarr* a = mutarr_create(TestFoo*);
    TestFoo foo = (TestFoo){.x=1, .y=2, .z=3, .f=3.14};
    TestFoo* r;
    int error;
    mutarr_append(a, &foo);
    error = mutarr_at(a, 0, (char*)&r);
    //TestFoo* r  = (TestFoo*)mutarr_get(a, 0);
    utest_assert(!error);
    utest_assert(r == &foo);
    utest_assert(r->x == foo.x);
    utest_assert(r->x == 1);
    return TestOk;
}

int test_static_strings(void) {
    return test_static_strings_append()
        +  test_static_strings_at()
        ;
}

int test_integral(void) {
     return test_long()
         +  test_char()
         ;
}

int main(void) {
    int failures = test_static_strings()
        + test_integral()
        + test_test_foo()
        ;
    if (failures) {
        fprintf(stderr, RED "itests: %d tests failed" RESET "\n", failures);
    } else {
        printf(GREEN "itests: all tests passed" RESET "\n");
    }
    return failures ? EXIT_FAILURE : EXIT_SUCCESS;
}

