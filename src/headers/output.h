#include "sorts_alternative.h"
#include "sorts_quadratic.h"
#include "sorts_nlogn.h"
#include "generic_sort.h"
#include "utils.h"

double test_sort (Sort sort, int* array, int size);
void print_table (long long int** data, double* times);
void benchmark (int* array, int size);
