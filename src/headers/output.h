#include "sorts_alternative.h"
#include "sorts_quadratic.h"
#include "sorts_nlogn.h"
#include "generic_sort.h"
#include "utils.h"

double test_sort (Sort sort, int* array, int size, bool kill_on_null);
void benchmark (int* array, int size, bool output_to_file, bool kill_on_null);
