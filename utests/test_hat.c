#include <utests.h>
#include <hat.h>

typedef_hat(int, int);

int test_hat_0(void) {
	hat_int_int* H = &hat_create(int, int);
	hat_init(H);
	utest_assert(!hat_err(H));
	return 0;
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
