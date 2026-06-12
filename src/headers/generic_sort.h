#include "utils.h"

#ifndef SORT_METRICS
#define SORT_METRICS

typedef struct {
    int size;
    int largest;
    int smallest;
    int direct_inversion_count;
    float mean;
    unsigned int highest_bit;
} Metrics;

#endif

#ifndef SORT_WEIGHT
#define SORT_WEIGHT

typedef enum {
    WEIGHT_ARITHMETIC = 1,
    WEIGHT_ASSIGN = 1,      
    WEIGHT_ALLOCATION = 1  
} Weight;

#endif

void get_metrics (const int* array, const int n, Metrics* met);
long long radix_score (const Metrics* met);
long long count_score (const Metrics* met);
long long merge_score (const Metrics* met);
long long quick_sort_score (const Metrics* met);
void best_sort (int* array, const int n);
SortMethod choose_sort (int* array, const int n);
