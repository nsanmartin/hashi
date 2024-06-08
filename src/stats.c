#include <math.h>
#include <limits.h>
#include <stddef.h>
#include <stdbool.h>

static inline bool is_sum_overflow(unsigned long long x, unsigned long long y) { return x > ULLONG_MAX - y; }


double std(size_t* xs, size_t n) {
    double sum = 0;
    double ss = 0;

    for (size_t i = 0; i < n; ++i) {
        sum += xs[i];
        ss += xs[i] * xs[i];;
    }

    double mean = sum / n;
    double ex2 = ss / n;
    return sqrt(ex2 - mean*mean);
}
