#include "utils.h"

typedef struct {
    int size;
    int largest;
    int smallest;
    int direct_inversion_count;
    float mean;
    unsigned int highest_bit;
} Metrics;

typedef enum {
    WEIGHT_ARITHMETIC = 1,
    WEIGHT_ASSIGN = 1,      
    WEIGHT_ALLOCATION = 1  
} Weight;

void get_metrics (const int* array, const int n, Metrics* met);
long long radix_score (const Metrics* met);
long long count_score (const Metrics* met);
long long merge_score (const Metrics* met);
long long quick_sort_score (const Metrics* met);
SortMethod choose_sort (int* array, const int n);
