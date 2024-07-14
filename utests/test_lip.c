#include <stdio.h>
#include <utests.h>

#define KT int
#define VT int
#include <lip.h>

int test_0(void) {
    int status = 1;
    LipOf(int,int)* x = &(LipOf(int,int)){0};
    x->zerok = 1;
    x->max_tries = 9;

    size_t initsz = 4;
    int err = lipfn(int,int,init)(x, initsz);
    utest_assert(!err, clean);
    utest_assert(lipsz(x) == initsz, clean);
    clean_and_ret(status, clean, lipfn(int,int,clean)(x));
}

int main(void) {
    int failures = 0
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		puts("Tests Ok!");
	}
	return failures;
}
