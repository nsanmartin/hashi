#include <assert.h>

#include <utests.h>
#include <da.h>


typedef_da(int);

typedef struct {
    int x[256];
    int y;
} Foo;

typedef_da(Foo);

int test_da_0(void) {

    da_of(int)* a = &(da_of(int)){0};
    assert(a);
    int x, y, err;
    if(x = 1, err = da_append(a, x)) { return 1; }
    if(x = 2, err = da_append(a, x)) { return 1; }

    if (da_at(a, 0, &y) || y != 1) { return 1; }
    if (da_at(a, 1, &y) || y != 2) { return 1; }

    char c;
    da_at(a, 0, &c);
    
    da_of(Foo)* b = &(da_of(Foo)){0};
    if(c = 1, err = da_append(b, x)) { return 1; }
    if (da_at(b, 0, &c)) { return 1; }
    return 0;
}


int main(void) {
    int failures = test_da_0()
        ;

	if (failures) {
		fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
	} else {
		puts("Tests Ok!");
	}
	return failures;
}
