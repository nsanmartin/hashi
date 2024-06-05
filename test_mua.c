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
	int* value = mua_at(x, 0);
	if (!value) { return 1; }
	if (*value != 1) { return 1; }

	mua_item_type(x)* value2 = mua_at(x, 9);
	if (value2) { return 1; }
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
		mua_item_type(x)* value = mua_at(x, i);
		utest_assert(value);
		utest_assert(*value == 3*(int)i);
	}
	
	return 0;
}


int test_mua_2(void) {
	mua_Foo* x = &mua_create(Foo);
	utest_assert(mua_len(x) == 0);
	mua_append(x, ((Foo){.x=4,.y=7,.f=0}));
	utest_assert(!mua_err(x));
	mua_item_type(x)* f = mua_at(x, 0);
	utest_assert(f);
	utest_assert(f->x == 4 && f->y == 7 && f->f == 0);
	return 0;
}

int test_mua_string(void) {
	mua_char_ptr* x = &mua_ptr_create(char);
	mua_append(x, "foo");
	utest_assert(!mua_err(x));
	mua_append(x, "bar");
	utest_assert(!mua_err(x));
	mua_item_type(x)* v = mua_at(x, 0);
	utest_assert(v);
	utest_assert_str_eq("foo", *v);
	v = mua_at(x, 1);
	utest_assert(v);
	utest_assert_str_eq("bar", *v);
	return 0;
}

int test_mua_find_int(void) {
	mua_int* x = &mua_create(int);
	mua_append(x, 13);
	utest_assert(!mua_err(x));
	mua_append(x, 9);
	utest_assert(!mua_err(x));
	mua_item_type(x)* it = mua_find(x, 13);
	utest_assert(it);
	utest_assert(*it == 13);

	it = mua_find(x, 9);
	utest_assert(it);
	utest_assert(*it == 9);

	it = mua_find(x, 1);
	utest_assert(!it);

	it = mua_find(x, 0);
	utest_assert(!it);
	return 0;
}

int main(void) {
	int failures = test_mua_0()
		+ test_mua_1()
		+ test_mua_2()
		+ test_mua_string()
		+ test_mua_find_int()
		;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		puts("Tests Ok!");
	}
}
