#include <limits.h>
#include <utests.h>
#include <rand-word.h>
#include <hat.h>

enum { Cien = 100 };

typedef_hat(int, int);
typedef_str_hat(int);


void str_hat_at_2(str_hat_int* H, char* K, str_hat_int_elem** OUT) { 
    hat_slot_type(H)* st; 
    str_hat_slot_for(H, K, &st); 

    arl_item_type(st)* it = 0x0;

    str_hat_int_elem* items = st->items;
    size_t isz = arl_item_size(st);
    size_t len = arl_len(st);
    char** citems = (char**)items;
    
    it = _arl_find_str_impl(citems, K, isz, len);

    *OUT = it; 
}

/*
 * Words from `help | awk '{ print $1 }' | sort | uniq`
 */
char* test_strings[] = {
    "((",
    ".",
    ":",
    "[",
    "[[",
    "A",
    "alias",
    "bg",
    "bind",
    "break",
    "builtin",
    "caller",
    "case",
    "cd",
    "command",
    "compgen",
    "complete",
    "compopt",
    "continue",
    "coproc",
    "declare",
    "dirs",
    "disown",
    "echo",
    "enable",
    "eval",
    "exec",
    "exit",
    "export",
    "false",
    "fc",
    "fg",
    "for",
    "function",
    "getopts",
    "GNU",
    "hash",
    "help",
    "job_spec",
    "These",
    "Type",
    "Use",
};

size_t test_strings_sz = sizeof(test_strings) / sizeof(test_strings[0]);

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
    utest_assert_clean(hat_size(H) == 0);

    hat_elem_type(H)*it = 0x0;
    str_hat_at(H, "foobar", &it);
    utest_assert_clean(!it);
    utest_assert_clean(!hat_err(H));

    utest_finally_and_return(((void)0));
}

int test_str_hat_1(void) {
    str_hat_int* H = &(str_hat_int){0};
    hat_init(H, 15);
    utest_assert(!hat_err(H));
    utest_assert_clean(!hat_err(H));

    str_hat_update(H, "foo", 3);
    utest_assert_clean(hat_size(H) == 1);
    hat_elem_type(H)*it = 0x0;
    str_hat_at(H, "foo", &it);
    utest_assert_clean(it);
    utest_assert_clean(!strcmp(it->k, "foo"));
    utest_assert_clean(it->v == 3);

    utest_finally_and_return(((void)0));
}

int test_str_hat_2(void) {
    str_hat_int* H = &(str_hat_int){0};
    hat_init(H, 5);
    utest_assert(!hat_err(H));
    utest_assert_clean(!hat_err(H));

    const int value = 7;
    str_hat_update(H, "foo", value);
    utest_assert_clean(hat_size(H) == 1);
    hat_elem_type(H)*it = 0x0;
    str_hat_at(H, "foo", &it);
    utest_assert_clean(it);
    utest_assert_clean(!strcmp(it->k, "foo"));
    utest_assert_clean(it->v == value);

    str_hat_update(H, "foo", value);
    utest_assert_clean(hat_size(H) == 1);
    it = 0x0;
    str_hat_at(H, "foo", &it);
    utest_assert_clean(it);
    utest_assert_clean(!strcmp(it->k, "foo"));
    utest_assert_clean(it->v == value);

    utest_finally_and_return(((void)0));
}

int test_str_hat_3(void) {
    str_hat_int* H = &(str_hat_int){0};
    hat_init(H, 1);
    utest_assert(!hat_err(H));
    utest_assert_clean(!hat_err(H));
    utest_assert_clean(hat_size(H) == 0);

    for (size_t i = 0; i < test_strings_sz; ++i) {
        char* s = test_strings[i]; 

        str_hat_update(H, s, (int)i);
        utest_assert_clean(hat_size(H) == i+1);
    }

    for (size_t i = 0; i < test_strings_sz; ++i) {
        char* s = test_strings[i]; 

        hat_elem_type(H)*it = 0x0;
        str_hat_at(H, s, &it);
        utest_assert_clean(it);
        utest_assert_clean(!strcmp(it->k, s));
        utest_assert_clean(it->v == (int)i);
    }
    utest_finally_and_return(((void)0));
}

int test_str_hat_4(void) {
    str_hat_int* H = &(str_hat_int){0};
    hat_init(H, 1);
    utest_assert(!hat_err(H));
    utest_assert_clean(!hat_err(H));

    char* words[] = { "foo", "bar" };

    str_hat_update(H, words[0], 0);
    utest_assert_clean(!hat_err(H));
    str_hat_update(H, words[1], 1);
    utest_assert_clean(!hat_err(H));

    hat_elem_type(H)*it = 0x0;
    str_hat_at(H, words[0], &it);
    utest_assert_clean(it);
    utest_assert_clean(!strcmp(it->k, words[0]));
    utest_assert_clean(it->v == 0);

    str_hat_at_2(H, words[1], &it);
    utest_assert_clean(it);
    utest_assert_clean(!strcmp(it->k, words[1]));
    utest_assert_clean(it->v == 1);

    str_hat_at(H, words[1], &it);
    utest_assert_clean(it);
    utest_assert_clean(!strcmp(it->k, words[1]));
    utest_assert_clean(it->v == 1);

    utest_finally_and_return(((void)0));
}


int test_str_hat_5(void) {
    str_hat_int* H = &(str_hat_int){0};
    hat_init(H, 5);
    utest_assert(!hat_err(H));
    utest_assert_clean(!hat_err(H));

    char w[Cien] = {0};

    for (int i = 0; i < Cien; ++i) {
        hat_update(H, w, i);
        utest_assert_clean(!hat_err(H));
    }


    utest_finally_and_return(((void)0));
}

int main(void) {
    int failures =
        test_hat_0()
        + test_hat_1()
        + test_str_hat_0()
        + test_str_hat_1()
        + test_str_hat_2()
        + test_str_hat_3()
        + test_str_hat_4()
        + test_str_hat_5()
        ;
    if (failures) {
        fprintf(stderr, "%d test%s failed\n", failures, (failures == 1 ? "" : "s"));
    } else {
        puts("Tests Ok!");
    }
    return failures;
}

