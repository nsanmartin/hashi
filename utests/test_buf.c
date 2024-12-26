#include <stdio.h>
#include <utests.h>

#define BT int
#include <buf.h>

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
    buffn(int, __extend_capacity)(x, x->len);
    for (; it != end; ++it) {
        utest_assert(it, clean);
        utest_assert(*it == 0, clean);
    }
    utest_assert(buflen(x) == initsz + initsz, clean);
    clean_and_ret(status, clean, buffn(int, clean)(x));
}

int main(void) {
    int failures = 0
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		printf("%s: Tests Ok!\n", __FILE__);
	}
	return failures;
}
