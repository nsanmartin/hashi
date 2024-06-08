#include <string.h>
#include <math.h>
#include <limits.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include <stats.h>

static inline bool is_sum_overflow(unsigned long long x, unsigned long long y) { return x > ULLONG_MAX - y; }

void stats_init(Stats* st, size_t* xs, size_t n) {
    *st = (Stats) {
       .n = n,
       .xs = xs,
       .min = SIZE_MAX,
       .max = 0,
       .sum = 0,
       .ss = 0,
       .mean = 0
    };

    for (size_t i = 0; i < st->n; ++i) {
        st->sum += xs[i];
        st->ss += xs[i] * xs[i];
        st->min = st->min > xs[i] ? xs[i] : st->min;
        st->max = st->max < xs[i] ? xs[i] : st->max;
    }

    st->mean = (double)st->sum / (double)st->n;
    st->stdev = sqrt((st->ss / st->n) - st->mean*st->mean);
}

double standard_deviation(size_t* xs, size_t n) {
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

double standard_deviation_2(size_t* xs, size_t n) {
    double sum = 0;
    double ss = 0;

    for (size_t i = 0; i < n; ++i) { sum += xs[i]; }
    double mean = sum / n;

    for (size_t i = 0; i < n; ++i) {
        double dist = xs[i] - mean;
        ss += dist * dist;
    }
    return sqrt(ss / n);
}

void print_freqs(Stats* st) {
    if (st->max == st->min) {
        puts("TODO: all eq :)");
        return;
    }

    size_t range = 1 + st->max - st->min;
    size_t frqs[range];
    memset(frqs, 0, sizeof(frqs));

    for (size_t i = 0; i < st->n; ++i) {
        ++frqs[st->xs[i] - st->min];
    }
    
    for (size_t i = 0; i < range; ++i) {
        printf("%ld: %ld\n", i + st->min, frqs[i]);
    }
}
