#include <limits.h>
#include <utests.h>
#include <hat.h>

typedef_hat(int, int);



int test_hat_0(void) {
    hat_int_int* H = &hat_create(int, int);
    hat_init(H);
    utest_assert(!hat_err(H));

    hat_update(H, INT_MAX, 10);
    typeof(H->_elem)* it;
    int v = INT_MAX;
    hat_at(H, v, it);
    utest_assert_clean(it);
    utest_assert_clean(it->k == v);

    v = 7;
    hat_at(H, v, it);
    utest_assert_clean(!it);
//TODO: cleanup
    utest_finally_and_return(((void)0));
}

int main(void) {
    int failures = test_hat_0();
    if (failures) {
        fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
    } else {
        puts("Tests Ok!");
    }
    return failures;
}
