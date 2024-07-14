#include <stdio.h>
#include <utests.h>

#define KT int
#define VT int
#include <lip.h>

int test_0(void) {
    LipOf(int,int) x = (LipOf(int,int)){0};
    x.zerok = 1;
    x.max_tries = 9;
    return 0;
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
