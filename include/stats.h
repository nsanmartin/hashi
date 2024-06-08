#ifndef __STATS_HASHI__H_
#define __STATS_HASHI__H_

typedef struct {
    size_t n;
    size_t* xs;
    size_t min;
    size_t max;
    double sum;
    double ss;
    double mean;
    double stdev;
} Stats;

void stats_init(Stats* st, size_t* xs, size_t n);
void print_freqs(Stats* st);
double standard_deviation(size_t* xs, size_t n);
double standard_deviation_2(size_t* xs, size_t n);
#endif
