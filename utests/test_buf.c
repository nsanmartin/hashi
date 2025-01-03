#include <stdio.h>
#include <utests.h>

#define BT int
#include <buf.h>

#define BT char
#include <buf.h>

/* calloc */
int test_0(void) {
    int status = 1;
    BufOf(int)* x = &(BufOf(int)){0};
    size_t initsz = 4;
    buffn(int, calloc)(x, initsz);
    int* it = buffn(int, begin)(x);
    int* end = buffn(int, end)(x);

    for (; it != end; ++it) {
        utest_assert(it, clean);
        utest_assert(*it == 0, clean);
    }

    utest_assert(buflen(x) == initsz, clean);
    clean_and_ret(status, clean, buffn(int, clean)(x));
}

/* append */
int test_1(void) {
    int status = 1;
    BufOf(int)* x = &(BufOf(int)){0};

    int arr[] = { 3,5,2,5,6,8, -1, -23232};
    size_t arr_size = sizeof arr / sizeof (*arr);
    int* p = arr;

    buffn(int, append)(x, p, arr_size);
    int* it = buffn(int, begin)(x);
    int* end = buffn(int, end)(x);

    for (; it != end; ++it, ++p) {
        utest_assert(it, clean);
        utest_assert(*it == *p, clean);
    }

    utest_assert((size_t)(p-arr) == arr_size, clean);
    utest_assert(8 == arr_size, clean);


    buffn(int, append)(x, arr, arr_size);
    it = buffn(int, begin)(x);
    end = buffn(int, end)(x);
    for (size_t i = 0; i < 8; ++it, ++i) {
        utest_assert(it, clean);
        utest_assert(*it == arr[i], clean);
    }
    for (size_t i = 0; i < 8; ++it, ++i) {
        utest_assert(it, clean);
        utest_assert(*it == arr[i], clean);
    }
    utest_assert((size_t)(it-x->items)/2 == arr_size, clean);
    utest_assert(16 == x->len, clean);

    clean_and_ret(status, clean, buffn(int, clean)(x));
}

/* reset */
int test_2(void) {
    int status = 1;
    BufOf(int)* x = &(BufOf(int)){0};

    int arr[] = { 3,5,2,5,6,8, -1, -23232};
    size_t arr_size = sizeof arr / sizeof (*arr);
    int* p = arr;

    buffn(int, append)(x, p, arr_size);
    buffn(int, reset)(x);
    utest_assert(x->len == 0, clean);


    buffn(int, append)(x, arr, arr_size);
    int* it = buffn(int, begin)(x);
    for (size_t i = 0; i < 8; ++it, ++i) {
        utest_assert(it, clean);
        utest_assert(*it == arr[i], clean);
    }
    utest_assert((size_t)(it-x->items) == arr_size, clean);
    utest_assert(8 == x->len, clean);
    int* end = buffn(int, end)(x);
    utest_assert(it == end, clean);

    clean_and_ret(status, clean, buffn(int, clean)(x));
}

int test_3_check(BufOf(int)*x, int* a, size_t n) {
    buffn(int, append)(x, a, n);

    int* it = buffn(int, begin)(x);
    for(size_t i = 0
        ; it != buffn(int, end)(x)
        ; ++it, ++i
    ) {
        utest_assert_ret(it); utest_assert_ret(*it == a[i]);
    }
    utest_assert_ret(n == (size_t)(it-buffn(int, begin)(x)));
    utest_assert_ret(n == x->len);

    buffn(int, reset)(x);
    return 0;
}

int test_3(void) {
    int status = 1;
    BufOf(int)* x = &(BufOf(int)){0};

    int a1[1] = { 3 };
    int a2[2] = { 3, 4 };
    int a3[3] = { 3, 4, 5 };
    int a4[4] = { 4, 5, 6, 7 };
    int a5[5] = { 4, 5, 6, 7 };
    int a6[6] = { 4, 5, 6, 7, 8 };
    int a7[7] = { 4, 5, 6, 7, 8, 9 };
    int a8[8] = { 4, 5, 6, 7, 8, 9, 10 };
    int a9[9] = { 4, 5, 6, 7, 8, 9, 10, 11 };

    utest_assert(!test_3_check(x, a1, sizeof a1 / sizeof(int)), clean);
    utest_assert(!test_3_check(x, a2, sizeof a2 / sizeof(int)), clean);
    utest_assert(!test_3_check(x, a3, sizeof a3 / sizeof(int)), clean);
    utest_assert(!test_3_check(x, a4, sizeof a4 / sizeof(int)), clean);
    utest_assert(!test_3_check(x, a5, sizeof a5 / sizeof(int)), clean);
    utest_assert(!test_3_check(x, a6, sizeof a6 / sizeof(int)), clean);
    utest_assert(!test_3_check(x, a7, sizeof a7 / sizeof(int)), clean);
    utest_assert(!test_3_check(x, a8, sizeof a8 / sizeof(int)), clean);
    utest_assert(!test_3_check(x, a9, sizeof a9 / sizeof(int)), clean);

    clean_and_ret(status, clean, buffn(int, clean)(x));
}

int test_4(void) {
    int status = 1;
    BufOf(int)* x = &(BufOf(int)){0};

    int arr[] = { 3,5,2,5,6,8, -1, -23232};
    size_t arr_size = sizeof arr / sizeof (*arr);
    int* p = arr;

    buffn(int, prepend)(x, p, arr_size);
    int* it = buffn(int, begin)(x);
    int* end = buffn(int, end)(x);

    for (; it != end; ++it, ++p) {
        utest_assert(it, clean);
        utest_assert(*it == *p, clean);
    }

    utest_assert((size_t)(p-arr) == arr_size, clean);
    utest_assert(8 == arr_size, clean);


    buffn(int, prepend)(x, arr, arr_size);
    it = buffn(int, begin)(x);
    end = buffn(int, end)(x);
    for (size_t i = 0; i < 8; ++it, ++i) {
        utest_assert(it, clean);
        utest_assert(*it == arr[i], clean);
    }
    for (size_t i = 0; i < 8; ++it, ++i) {
        utest_assert(it, clean);
        utest_assert(*it == arr[i], clean);
    }
    utest_assert((size_t)(it-x->items)/2 == arr_size, clean);
    utest_assert(16 == x->len, clean);

    clean_and_ret(status, clean, buffn(int, clean)(x));
}

int test_5(void) {
    int* elem;
    int status = 1;
    BufOf(int)* x = &(BufOf(int)){0};

    //            0   1   2   3   4  5  6  7  8  9  10
    int arr[] = {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5};

    buffn(int, append)(x, arr+5, 1);
    elem = buffn(int, at)(x, 0);
    utest_assert(elem, clean);
    utest_assert(*elem == 0, clean);

    buffn(int, prepend)(x, arr+1, 4);
    elem = buffn(int, at)(x, 0);
    utest_assert(elem, clean);
    utest_assert(*elem == -4, clean);

    elem = buffn(int, at)(x, 4);
    utest_assert(elem, clean);
    utest_assert(*elem == 0, clean);

    buffn(int, append)(x, arr+6, 4);
    elem = buffn(int, at)(x, 0);
    utest_assert(elem, clean);
    utest_assert(*elem == -4, clean);

    elem = buffn(int, at)(x, 7);
    utest_assert(elem, clean);
    utest_assert(*elem == 3, clean);

    buffn(int, prepend)(x, arr, 1);
    elem = buffn(int, at)(x, 0);
    utest_assert(elem, clean);
    utest_assert(*elem == -5, clean);

    clean_and_ret(status, clean, buffn(int, clean)(x));
}

int test_6(void) {
    int status = 1;
    BufOf(char)* x = &(BufOf(char)){0};

    char* first = "First";
    char* second = "Second";
    char* third = "Third";
    char* fourth = "Fourth";


    buffn(char, prepend)(x, fourth, sizeof fourth);
    buffn(char, prepend)(x, third, sizeof third);
    buffn(char, prepend)(x, second, sizeof second);
    buffn(char, prepend)(x, first, sizeof first);
    buffn(char, append)(x, "\0", 1);
    utest_assert(x->items, clean);
    utest_assert(!strcmp(x->items, "First" "Second" "Third" "Fourth"), clean);

    buffn(char, reset)(x);

    buffn(char, prepend)(x, fourth, sizeof fourth);
    buffn(char, prepend)(x, third, sizeof third);
    buffn(char, prepend)(x, second, sizeof second);
    buffn(char, prepend)(x, first, sizeof first);
    buffn(char, append)(x, "\0", 1);
    utest_assert(x->items, clean);
    utest_assert(!strcmp(x->items, "First" "Second" "Third" "Fourth"), clean);
    clean_and_ret(status, clean, buffn(char, clean)(x));
}

int main(void) {
    int failures
        = test_0()
        + test_1()
        + test_2()
        + test_3()
        + test_4()
        + test_5()
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		printf("%s: Tests Ok!\n", __FILE__);
	}
	return failures;
}
