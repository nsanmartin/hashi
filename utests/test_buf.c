#include <stdio.h>
#include <utests.h>

#define BT int
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

int main(void) {
    int failures = test_0()
        + test_1()
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		printf("%s: Tests Ok!\n", __FILE__);
	}
	return failures;
}
