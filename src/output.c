#include "headers/output.h"

double test_sort(Sort sort, int* array, int size) {
    time_t begin = clock();
    sort(array, size);
    time_t end = clock();

    double dt = (end - begin) / (double) CLOCKS_PER_SEC;

    if (!is_sorted(array, size)) {
        print_array(array, size);
        printf("\nAs you can see, it is not sorted...\nI'll interrupt it here just to get your attention\n");
        exit(0);
    }
    return dt;
}

void print_table (long long int** data, double* times) {
    printf( "                    |    Quick    |    Merge    |    Bubble   |  Selection  |  Insertion  |    Count    | Bytewise Radix\n"
            "Time Taken          | %10lfs | %10lfs | %10lfs | %10lfs | %10lfs | %10lfs | %10lfs \n"
            "Comparisons         | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n"
            "Assignments         | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n"
            "Max Recursion Depth | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n"
            "Recursive Calls     | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n",
            // Must cast it back so it uses the right float register
            times[0], times[1], times[2], times[3], times[4], times[5], times[6],
            data[0][1], data[1][1], data[2][1], data[3][1], data[4][1], data[5][1], data[6][1],
            data[0][2], data[1][2], data[2][2], data[3][2], data[4][2], data[5][2], data[6][2],
            data[0][3], data[1][3], data[2][3], data[3][3], data[4][3], data[5][3], data[6][3],
            data[0][4], data[1][4], data[2][4], data[3][4], data[4][4],  data[5][4], data[6][4]
          );
}

void benchmark (int* array, int size) {
    printf( "Benchmark mode\n"
            "Sorting algorithms available:\n"
            "\tQuick Sort\n"
            "\tMerge Sort\n"
            "\tBubble Sort\n"
            "\tSelection Sort\n"
            "\tInsertion Sort\n"
            "\tCount Sort\n"
            "\tBytewise Radix Sort\n"
            "Results:\n"
            );
    // Aloca matrix de dados nº Sorts x nº métricas
    long long int** data = (long long int**) calloc(7, sizeof(long long int*));
    double* times = (double*) calloc(7, sizeof(double));
    for (int i = 0; i < 7; i++)
        data[i] = (long long int*) calloc(4, sizeof(long long int));

    // Collect data
    double dt;
    Sort sort;
    int* array_copy;
    for (int i = 0; i < 7; i++) {
        array_copy = copy_array(array, size);
        switch (i) {
        case 0: default:
            sort = quick_sort;
            break;
        case 1:
            sort = merge_sort;
            break;
        case 2:
            sort = bubble_sort;
            break;
        case 3:
            sort = selection_sort;
            break;
        case 4:
            sort = insertion_sort;
            break;
        case 5:
            sort = count_sort;
            break;
        case 6:
            sort = bytewise_radix_sort;
            break;
        }
        // Store data
        dt = test_sort(sort, array, size);
        times[i] = dt; // Just so it works; it's lazy, I know
        data[i][0] = ctr_compare;
        data[i][1] = ctr_assign;
        data[i][2] = ctr_recursion_depth;
        data[i][3] = ctr_recursion_call;
        clear_counters();
        free(array_copy);
    }
    print_table(data, times);
    for (int i = 0; i < 7; i++)
        free(data[i]);
    free(data);
    free(times);
}
