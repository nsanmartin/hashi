#include <mua.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

enum { TestOk = 0, TestFail = 1 };

#define utest_assert(Expr)  \
    if (!(Expr)) { fprintf(stderr, RED "Test failed: %s" RESET "\n", __func__); return TestFail; }

#define utest_assert_str_eq(Expected, String) \
    utest_assert(strcmp(Expected, String) == 0)

typedef_mua(int);

int test_mua_0(void) {
	mua_int* x = &mua_create(int);
	mua_append(x, 1);
	if (mua_err(x)) { return 1; }
	int value;
	int e = mua_at(x, 0, value);
	if (e) { return 1; }
	if (value != 1) {
		return 1;
	}

	e = mua_at(x, 9, value);
	if (!e) { return 1; }
	return 0;
}

int test_mua_1(void) {
	const size_t test_len = 10000;
	mua_int* x = &mua_create(int);
	for (size_t i = 0; !mua_err(x) && i < test_len; ++i) {
		mua_append(x, 3*i);
	}
	utest_assert(!mua_err(x));
	utest_assert(mua_len(x) == test_len);
	for (size_t i = 0; !mua_err(x) && i < test_len; ++i) {
		int value;
		int e = mua_at(x, i, value);
		utest_assert(!e);
		utest_assert(value == 3*(int)i);
	}
	
	return 0;
}

int main(void) {
	int failures = test_mua_0()
		+ test_mua_1();

	if (failures) {
		fprintf(stderr, "%d test%s failed", failures, (failures == 1 ? "" : "s"));
	} else {
		puts("Tests Ok!");
	}
}
