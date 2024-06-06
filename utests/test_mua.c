#include <utests.h>
#include <mua.h>

#define utest_mua_assert_find_str(Mua, V) do{ \
	mua_item_type(x) __value_ = V; \
	mua_item_type(x)* it = mua_find(x, __value_); \
	utest_assert_clean(it); \
	utest_assert_clean(strcmp(*it, V) == 0); \
} while(0)

#define utest_mua_assert_find(Mua, V) do{ \
	mua_item_type(x) __value_ = V; \
	mua_item_type(x)* it = mua_find(x, __value_); \
	utest_assert_clean(it); \
	utest_assert_clean(*it == V); \
} while(0)

#define utest_mua_assert_find_not(Mua, V) do{ \
	mua_item_type(x) __value_ = V; \
	mua_item_type(x)* it = mua_find(x, __value_); \
	utest_assert_clean(!it); \
} while(0)


typedef_mua(int);
typedef struct { int x, y; float f; } Foo;
typedef_mua(Foo);
typedef_mua_ptr(char);
typedef_mua_ptr(Foo);

int test_mua_0(void) {
	mua_int* x = &mua_create(int);
	mua_append(x, 1);
	utest_assert(!mua_err(x));
	mua_item_type(x)* value = mua_at(x, 9);
	utest_assert_clean(!value);
	value = mua_at(x, 0);
	utest_assert_clean (value);
	utest_assert_clean(*value == 1);

	utest_finally_and_return(mua_cleanup(x));
}

int test_mua_1(void) {
	const size_t test_len = 10000;
	mua_int* x = &mua_create(int);
	for (size_t i = 0; !mua_err(x) && i < test_len; ++i) {
		mua_append(x, 3*i);
		if (i) { utest_assert_clean(x); } else { utest_assert(x); }
	}
	utest_assert_clean(mua_len(x) == test_len);
	for (size_t i = 0; !mua_err(x) && i < test_len; ++i) {
		mua_item_type(x)* value = mua_at(x, i);
		utest_assert_clean(value);
		utest_assert_clean(*value == 3*(int)i);
	}
	
	utest_finally_and_return(mua_cleanup(x));
}


int test_mua_2(void) {
	mua_Foo* x = &mua_create(Foo);
	utest_assert(mua_len(x) == 0);
	mua_append(x, ((Foo){.x=4,.y=7,.f=0}));
	utest_assert(!mua_err(x));
	mua_item_type(x)* f = mua_at(x, 0);
	utest_assert_clean(f);
	utest_assert_clean(f->x == 4 && f->y == 7 && f->f == 0);

	utest_finally_and_return(mua_cleanup(x));
}

int test_mua_string(void) {
	mua_char_ptr* x = &mua_ptr_create(char);
	mua_append(x, "foo");
	utest_assert(!mua_err(x));

	mua_append(x, "bar");
	utest_assert_clean(!mua_err(x));
	mua_item_type(x)* v = mua_at(x, 0);
	utest_assert_clean(v);
	utest_assert_str_eq_clean("foo", *v);
	v = mua_at(x, 1);
	utest_assert_clean(v);
	utest_assert_str_eq_clean("bar", *v);

	utest_finally_and_return(mua_cleanup(x));
}

int test_mua_find_int(void) {
	mua_int* x = &mua_create(int);
	mua_append(x, 13);
	utest_assert(!mua_err(x));
	mua_append(x, 9);
	utest_assert_clean(!mua_err(x));

	
	utest_mua_assert_find(x, 13);
	utest_mua_assert_find(x, 9);

	utest_mua_assert_find_not(x, 1);
	utest_mua_assert_find_not(x, 0);

	utest_finally_and_return(mua_cleanup(x));
}

int test_mua_find_string(void) {
	mua_char_ptr* x = &mua_ptr_create(char);
	mua_append(x, "foo");
	utest_assert(!mua_err(x));

	mua_append(x, "bar");
	utest_assert_clean(!mua_err(x));

	utest_mua_assert_find_str(x, "foo");
	utest_mua_assert_find_str(x, "bar");

	utest_mua_assert_find_not(x, "1");
	utest_mua_assert_find_not(x, "");

	utest_finally_and_return(mua_cleanup(x));
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
	return failures;
}
