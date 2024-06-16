#include <limits.h>
#include <utests.h>
#include <hat.h>

typedef_hat(int, int);
typedef_str_hat(int);


int test_hat_0(void) {
    hat_int_int* H = &hat_empty(int, int);
    hat_init(H, 10);
    utest_assert(!hat_err(H));

    hat_elem_type(H)*it = 0x0;
    utest_assert_clean(!it);
    hat_at(H, INT_MAX, &it);
    utest_assert_clean(!it);

    hat_at(H, 0, &it);
    utest_assert_clean(!it);

    hat_at(H, 7, &it);
    utest_assert_clean(!it);

    hat_update(H, -3, 3);
    hat_at(H, -3, &it);
    utest_assert_clean(it);
    utest_assert_clean(it->k == -3);
    utest_assert_clean(it->v == 3);

    utest_finally_and_return(((void)0));
}

int test_hat_1(void) {
    hat_int_int* H = &hat_empty(int, int);
    hat_init(H, 10);
    utest_assert(!hat_err(H));

    const int sz = 100;
    for (int i = 1; i < sz; ++i) {
        hat_update(H, -i, i);
    }

    for (int i = 1; i < sz; ++i) {
        hat_elem_type(H)*it = 0x0;
        hat_at(H, -i, &it);
        utest_assert_clean(it->k == -i);
        utest_assert_clean(it->v == i);
    }

    utest_finally_and_return(((void)0));
}

int test_str_hat_0(void) {
    str_hat_int* H = &(str_hat_int){0};
    hat_init(H, 10);
    utest_assert(!hat_err(H));

    hat_elem_type(H)*it = 0x0;
    str_hat_at(H, "foobar", &it);
    utest_assert_clean(!it);
    utest_assert_clean(!hat_err(H));

    utest_finally_and_return(((void)0));
}

int test_str_hat_1(void) {
    str_hat_int* H = &(str_hat_int){0};
    hat_init(H, 5);
    utest_assert(!hat_err(H));
    utest_assert_clean(!hat_err(H));

    str_hat_update(H, "foo", 3);
    hat_elem_type(H)*it = 0x0;
    str_hat_at(H, "foo", &it);
    utest_assert_clean(it);
    //utest_assert_clean(!strcmp(it->k, "foo"));
    //utest_assert_clean(it->v == 3);

    utest_finally_and_return(((void)0));
}


int main(void) {
    int failures = test_hat_0()
        + test_hat_1()
        + test_str_hat_0()
        + test_str_hat_1()
        ;
    if (failures) {
        fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
    } else {
        puts("Tests Ok!");
    }
    return failures;
}
