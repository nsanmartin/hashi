#include <stdbool.h>
#include <limits.h>
#include <utests.h>
#include <hat.h>

typedef_hat(int, int);

#define utest_hat_assert_update_at(Hat, K, V) do {\
    hat_update(H, K, V); \
    typeof(H->_k) __key_ = K; \
    typeof(H->_elem)* __it_; \
    hat_at(H, __key_, __it_); \
    utest_assert_clean(__it_); \
    utest_assert_clean(__it_->k == __key_); \
} while(0)

#define utest_hat_assert_not_at(Hat, K) do {\
    typeof(H->_k) __key_ = K; \
    typeof(H->_elem)* __it_; \
    hat_at(H, __key_, __it_); \
    utest_assert_clean(!__it_); \
} while(0)

bool check_update_at(hat_int_int* H, int k, int v) {
    hat_update(H, k, v); \
    typeof(H->_elem)* it; 
    hat_at(H, k, it); 
    return it != 0x0; 
}

//int test_hat_0(void) {
//    hat_int_int* H = &(hat_int_int){0};
//    do{
//	    (H)->len = 0;
//	    do {
//		    (&(H)->tab)->capacity = (&(H)->tab)->capacity ? 2 * (&(H)->tab)->capacity : MuaInitialCapacity ; (&(H)->tab)->items = realloc((&(H)->tab)->items, (&(H)->tab)->capacity * sizeof(*(&(H)->tab)->items)); if (!(&(H)->tab)->items) { perror("realloc failed"); do { (&(H)->tab)->capacity = 0; (&(H)->tab)->len = 1; } while(0); }
//	    } while(0);
//	    ((&(H)->tab)->len) = (&(H)->tab)->capacity;
//    } while(0);;
//
//    if (!(!(((&(H)->tab)->capacity == 0 && (&(H)->tab)->len == 1 )))) {
//	    fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__);
//	    return TestFail;
//    };
//
//
//
//
//    do {
//	    do {
//		    char __hat_k[sizeof(0)];
//		    typeof(0) __hat_k_lv = 0;
//		    memcpy(__hat_k, (char*)&__hat_k_lv, sizeof(0));
//		    size_t __hat_h = (djb2_k33_len((unsigned char*)__hat_k, sizeof(0)) % (&(H)->tab)->capacity) % (&(H)->tab)->capacity;
//		    typeof(*(&(H)->tab)->items)* __hat_bkt;
//		    __hat_bkt = ((__hat_h >= (&(H)->tab)->len) ? (0x0) : &((&(H)->tab)->items[__hat_h]));
//		    do {
//			    if ((__hat_bkt)->len >= (__hat_bkt)->capacity) {
//				    do {
//					    (__hat_bkt)->capacity = (__hat_bkt)->capacity ? 2 * (__hat_bkt)->capacity : MuaInitialCapacity ;
//					    (__hat_bkt)->items = realloc((__hat_bkt)->items, (__hat_bkt)->capacity * sizeof(*(__hat_bkt)->items));
//					    if (!(__hat_bkt)->items) {
//						    perror("realloc failed");
//						    do { (__hat_bkt)->capacity = 0; (__hat_bkt)->len = 1; } while(0);
//					    }
//				    } while(0);
//			    } if (!((__hat_bkt)->capacity == 0 && (__hat_bkt)->len == 1 ))
//				    (__hat_bkt)->items[(__hat_bkt)->len++] = ((typeof((H)->_elem)){.k=0,.v=11});
//		    } while(0);
//	    } while (0); typeof(H->_k) __key_ = 0; typeof(H->_elem)* __it_; do { typeof(__key_) __hat_k = __key_; size_t __hat_h = (djb2_k33_len((unsigned char*)(char*)&__hat_k, sizeof(__key_)) % (&(H)->tab)->capacity) % (&(H)->tab)->capacity; typeof(*(&(H)->tab)->items)* __hat_bkt; __hat_bkt = ((__hat_h >= (&(H)->tab)->len) ? (0x0) : &((&(H)->tab)->items[__hat_h])); typeof(*(__hat_bkt)->items)* __hat_it = _mua_find_impl((char*)(__hat_bkt)->items, (char*)&__key_, sizeof((H)->_k), ((__hat_bkt)->len)); __it_ = __hat_it; } while(0); if (!(__it_)) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup; }; if (!(__it_->k == __key_)) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup; };
//    } while(0);
//
//    if (!(check_update_at(H, 8, 1))) {
//	    fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup;
//    };
//
//
//
//
//
//
//    do { ((void)0); return TestOk; fail_cleanup: ((void)0); return TestFail; } while(0);
//}

//int test_hat_0(void) {
//    hat_int_int* H = &hat_create(int, int);
//    hat_init(H);
//    utest_assert(!hat_err(H));
//
//
//    utest_hat_assert_update_at(H, -1, 11);
//    utest_hat_assert_update_at(H, 0, 11);
//    utest_hat_assert_update_at(H, 8, 11);
//
////TODO: cleanup
//    utest_finally_and_return(((void)-1));
//}

//int test_hat_0(void) {
//    hat_int_int* H = &(hat_int_int){0};
//    do{ (H)->len = 0; do { (&(H)->tab)->capacity = (&(H)->tab)->capacity ? 2 * (&(H)->tab)->capacity : MuaInitialCapacity ; (&(H)->tab)->items = realloc((&(H)->tab)->items, (&(H)->tab)->capacity * sizeof(*(&(H)->tab)->items)); if (!(&(H)->tab)->items) { perror("realloc failed"); do { (&(H)->tab)->capacity = 0; (&(H)->tab)->len = 1; } while(0); } } while(0); ((&(H)->tab)->len) = (&(H)->tab)->capacity; } while(0);; if (!(!(((&(H)->tab)->capacity == 0 && (&(H)->tab)->len == 1 )))) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); return TestFail; };
//
//
//    do { do { char __hat_k[sizeof(-1)]; typeof(-1) __hat_k_lv = -1; memcpy(__hat_k, (char*)&__hat_k_lv, sizeof(-1)); size_t __hat_h = (djb2_k33_len((unsigned char*)__hat_k, sizeof(-1)) % (&(H)->tab)->capacity) % (&(H)->tab)->capacity; typeof(*(&(H)->tab)->items)* __hat_bkt; __hat_bkt = ((__hat_h >= (&(H)->tab)->len) ? (0x0) : &((&(H)->tab)->items[__hat_h])); do { if ((__hat_bkt)->len >= (__hat_bkt)->capacity) { do { (__hat_bkt)->capacity = (__hat_bkt)->capacity ? 2 * (__hat_bkt)->capacity : MuaInitialCapacity ; (__hat_bkt)->items = realloc((__hat_bkt)->items, (__hat_bkt)->capacity * sizeof(*(__hat_bkt)->items)); if (!(__hat_bkt)->items) { perror("realloc failed"); do { (__hat_bkt)->capacity = 0; (__hat_bkt)->len = 1; } while(0); } } while(0); } if (!((__hat_bkt)->capacity == 0 && (__hat_bkt)->len == 1 )) (__hat_bkt)->items[(__hat_bkt)->len++] = ((typeof((H)->_elem)){.k=-1,.v=11}); } while(0); } while (0); typeof(H->_k) __key_ = -1; typeof(H->_elem)* __it_; do { typeof(__key_) __hat_k = __key_; size_t __hat_h = (djb2_k33_len((unsigned char*)(char*)&__hat_k, sizeof(__key_)) % (&(H)->tab)->capacity) % (&(H)->tab)->capacity; typeof(*(&(H)->tab)->items)* __hat_bkt; __hat_bkt = ((__hat_h >= (&(H)->tab)->len) ? (0x0) : &((&(H)->tab)->items[__hat_h])); typeof(*(__hat_bkt)->items)* __hat_it = _mua_find_impl((char*)(__hat_bkt)->items, (char*)&__key_, sizeof((H)->_k), ((__hat_bkt)->len)); __it_ = __hat_it; } while(0); if (!(__it_)) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup; }; if (!(__it_->k == __key_)) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup; }; } while(0);
//
//    do {
//	    do {
//		    char __hat_k[sizeof(0)];
//		    typeof(0) __hat_k_lv = 0;
//		    memcpy(__hat_k, (char*)&__hat_k_lv, sizeof(0));
//		    size_t __hat_h = (djb2_k33_len((unsigned char*)__hat_k, sizeof(0)) % (&(H)->tab)->capacity) % (&(H)->tab)->capacity;
//		    typeof(*(&(H)->tab)->items)* __hat_bkt;
//		    __hat_bkt = ((__hat_h >= (&(H)->tab)->len) ? (0x0) : &((&(H)->tab)->items[__hat_h]));
//		    do {
//			    if ((__hat_bkt)->len >= (__hat_bkt)->capacity) {
//				    do {
//					    (__hat_bkt)->capacity = (__hat_bkt)->capacity ? 2 * (__hat_bkt)->capacity : MuaInitialCapacity ;
//					    (__hat_bkt)->items = realloc((__hat_bkt)->items, (__hat_bkt)->capacity * sizeof(*(__hat_bkt)->items));
//					    if (!(__hat_bkt)->items) {
//						    perror("realloc failed");
//						    do { (__hat_bkt)->capacity = 0; (__hat_bkt)->len = 1; } while(0);
//					    }
//				    } while(0);
//			    } if (!((__hat_bkt)->capacity == 0 && (__hat_bkt)->len == 1 )) (__hat_bkt)->items[(__hat_bkt)->len++] = ((typeof((H)->_elem)){.k=0,.v=11});
//		    } while(0);
//	    } while (0);
//	    typeof(H->_k) __key_ = 0;
//	    typeof(H->_elem)* __it_;
//	    do {
//		    typeof(__key_) __hat_k = __key_;
//		    size_t __hat_h = (djb2_k33_len((unsigned char*)(char*)&__hat_k, sizeof(__key_)) % (&(H)->tab)->capacity) % (&(H)->tab)->capacity;
//		    typeof(*(&(H)->tab)->items)* __hat_bkt;
//		    __hat_bkt = ((__hat_h >= (&(H)->tab)->len) ? (0x0) : &((&(H)->tab)->items[__hat_h]));
//		    typeof(*(__hat_bkt)->items)* __hat_it = _mua_find_impl((char*)(__hat_bkt)->items, (char*)&__key_, sizeof((H)->_k), ((__hat_bkt)->len));
//		    __it_ = __hat_it;
//	    } while(0);
//	    if (!(__it_)) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup; };
//	    if (!(__it_->k == __key_)) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup; };
//    } while(0);
//
//    do {
//	    do {
//		    char __hat_k[sizeof(8)];
//		    typeof(8) __hat_k_lv = 8;
//		    memcpy(__hat_k, (char*)&__hat_k_lv, sizeof(8));
//		    size_t __hat_h = (djb2_k33_len((unsigned char*)__hat_k, sizeof(8)) % (&(H)->tab)->capacity) % (&(H)->tab)->capacity;
//		    typeof(*(&(H)->tab)->items)* __hat_bkt;
//		    __hat_bkt = ((__hat_h >= (&(H)->tab)->len) ? (0x0) : &((&(H)->tab)->items[__hat_h]));
//		    do {
//			    if ((__hat_bkt)->len >= (__hat_bkt)->capacity) {
//				    do {
//					    (__hat_bkt)->capacity = (__hat_bkt)->capacity ? 2 * (__hat_bkt)->capacity : MuaInitialCapacity ;
//					    (__hat_bkt)->items = realloc((__hat_bkt)->items, (__hat_bkt)->capacity * sizeof(*(__hat_bkt)->items));
//					    if (!(__hat_bkt)->items) { perror("realloc failed"); do { (__hat_bkt)->capacity = 0; (__hat_bkt)->len = 1; } while(0); }
//				    } while(0);
//			    }
//			    if (!((__hat_bkt)->capacity == 0 && (__hat_bkt)->len == 1 )) (__hat_bkt)->items[(__hat_bkt)->len++] = ((typeof((H)->_elem)){.k=8,.v=11});
//		    } while(0);
//	    } while (0);
//	    typeof(H->_k) __key_ = 8;
//	    typeof(H->_elem)* __it_;
//	    do {
//		    typeof(__key_) __hat_k = __key_;
//		    size_t __hat_h = (djb2_k33_len((unsigned char*)(char*)&__hat_k, sizeof(__key_)) % (&(H)->tab)->capacity) % (&(H)->tab)->capacity;
//		    typeof(*(&(H)->tab)->items)* __hat_bkt;
//		    __hat_bkt = ((__hat_h >= (&(H)->tab)->len) ? (0x0) : &((&(H)->tab)->items[__hat_h]));
//		    typeof(*(__hat_bkt)->items)* __hat_it = _mua_find_impl((char*)(__hat_bkt)->items, (char*)&__key_, sizeof((H)->_k), ((__hat_bkt)->len));
//		    __it_ = __hat_it;
//	    } while(0);
//	    if (!(__it_)) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup; };
//	    if (!(__it_->k == __key_)) { fprintf( stderr , "\033[31m" "Test failed: %s" "\033[0m" "\n", __func__); goto fail_cleanup; };
//    } while(0);
//
//
//    do { ((void)-1); return TestOk; fail_cleanup: ((void)-1); return TestFail; } while(0);
//}





int test_hat_0(void) {
    hat_int_int* H = &hat_create(int, int);
    hat_init(H);
    utest_assert(!hat_err(H));

    utest_hat_assert_update_at(H, INT_MAX, 10);

    for (int i = 0; i < 1000; ++i) {
	    utest_hat_assert_not_at(H, i);
    }

    utest_hat_assert_update_at(H, 0, 11);
    utest_hat_assert_update_at(H, 7, 11);

    for (int i = 7; i < 100; ++i) {
	    utest_hat_assert_update_at(H, i, i * 3);
    }

    for (int i = 101; i < 701; ++i) {
	    utest_hat_assert_not_at(H, i);
    }

    utest_hat_assert_update_at(H, 11, 1);
    utest_hat_assert_update_at(H, 12, 1);

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
