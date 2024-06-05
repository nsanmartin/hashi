#include <mua.h>
#include <string.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"

enum { TestOk = 0, TestFail = 1 };

#define utest_assert(Expr)  \
    if (!(Expr)) { fprintf(stderr, RED "Test failed: %s" RESET "\n", __func__); return TestFail; }

#define utest_assert_str_eq(Expected, String) \
    utest_assert(strcmp(Expected, String) == 0)

typedef_mua(int);
typedef struct { int x, y; float f; } Foo;
typedef_mua(Foo);
typedef_mua_ptr(char);
typedef_mua_ptr(Foo);

int test_mua_0(void) {
	mua_int* x = &mua_create(int);
	mua_append(x, 1);
	if (mua_err(x)) { return 1; }
	int value;
	int e = mua_at(x, 0, &value);
	if (e) { return 1; }
	if (value != 1) {
		return 1;
	}

	e = mua_at(x, 9, &value);
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
		int e = mua_at(x, i, &value);
		utest_assert(!e);
		utest_assert(value == 3*(int)i);
	}
	
	return 0;
}


int test_mua_2(void) {
	mua_Foo* x = &mua_create(Foo);
	utest_assert(mua_len(x) == 0);
	mua_append(x, ((Foo){.x=4,.y=7,.f=0}));
	utest_assert(!mua_err(x));
	Foo f;
	utest_assert(!mua_at(x, 0, &f));
	utest_assert(f.x == 4 && f.y == 7 && f.f == 0);
	return 0;
}

int test_mua_string(void) {
	mua_char_ptr* x = &mua_ptr_create(char);
	mua_append(x, "foo");
	utest_assert(!mua_err(x));
	mua_append(x, "bar");
	utest_assert(!mua_err(x));
	char* v;
	utest_assert(!mua_at(x, 0, &v));
	utest_assert_str_eq("foo", v);
	utest_assert(!mua_at(x, 1, &v));
	utest_assert_str_eq("bar", v);
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
