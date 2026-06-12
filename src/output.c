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
    printf( "                    |    Quick    |    Merge    |    Bubble   |  Selection  |  Insertion  |  Counting   | Bytewise Radix\n"
            "Time Taken          | %10lfs | %10lfs | %10lfs | %10lfs | %10lfs | %10lfs | %10lfs \n"
            "Comparisons         | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n"
            "Assignments         | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n"
            "Max Recursion Depth | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n"
            "Recursive Calls     | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n"
            "Memory Allocated    | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld | %11lld \n",
            // Must cast it back so it uses the right float register
            times[0], times[1], times[2], times[3], times[4], times[5], times[6],
            data[0][0], data[1][0], data[2][0], data[3][0], data[4][0], data[5][0], data[6][0],
            data[0][1], data[1][1], data[2][1], data[3][1], data[4][1], data[5][1], data[6][1],
            data[0][2], data[1][2], data[2][2], data[3][2], data[4][2], data[5][2], data[6][2],
            data[0][3], data[1][3], data[2][3], data[3][3], data[4][3], data[5][3], data[6][3],
            data[0][4], data[1][4], data[2][4], data[3][4], data[4][4], data[5][4], data[6][4]
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
            "\tCounting Sort\n"
            "\tBytewise Radix Sort\n"
            "Results:\n"
            "Tamanho do array: %d\n", size
            );

    // Vê qual sort a heurística escolheria
    Sort sort; // Só usa realmente depois
    char* name = (char*)calloc(32, sizeof(char));
    SortMethod heuristic_sort = choose_sort(array, size), best_sort;
    get_sort_function(heuristic_sort, &sort, name);
    printf("Algoritmo escolhido pela heurística: %s\n", name);

    // Aloca matrix de dados nº Sorts x nº métricas
    // data[x][0] : comparações do sort x
    // data[x][1] : atribuições do sort x
    // data[x][2] : altura da árvore de recursão do sort x
    // data[x][3] : número de nós na árvore de recursão do sort x
    // data[x][4] : quantidade de INTs alocados
    long long int** data = calloc(7, sizeof(*data));
    double* times = calloc(7, sizeof(*times));
    for (int i = 0; i < 7; i++)
        data[i] = calloc(5, sizeof(*data[i]));

    // Collect data
    double dt, delta, heuristic_time, best_time = -1;
    int* array_copy;
    for (int i = 0; i < 7; i++) {
        SortMethod sort_name;
        array_copy = copy_array(array, size);
        switch (i) {
        case 0: default:
            sort = quick_sort;
            sort_name = SORT_QUICK;
            break;
        case 1:
            sort = merge_sort;
            sort_name = SORT_MERGE;
            break;
        case 2:
            sort = bubble_sort;
            sort_name = SORT_BUBBLE;
            break;
        case 3:
            sort = selection_sort;
            sort_name = SORT_SELECTION;
            break;
        case 4:
            sort = insertion_sort;
            sort_name = SORT_INSERTION;
            break;
        case 5:
            sort = count_sort;
            sort_name = SORT_COUNT;
            break;
        case 6:
            sort = bytewise_radix_sort;
            sort_name = SORT_RADIX;
            break;
        }
        // Store data
        dt = test_sort(sort, array_copy, size);
        times[i] = dt;
        data[i][0] = ctr_compare;
        data[i][1] = ctr_assign;
        data[i][2] = ctr_recursion_depth;
        data[i][3] = ctr_recursion_call;
        data[i][4] = ctr_mem_alloc;
        // Compare with heuristic
        if (best_time < 0) // First case only
            best_time = dt;
        else if (best_time > dt) {
            best_time = dt;
            best_sort = sort_name;
        }
        if (heuristic_sort == sort_name)
            heuristic_time = dt;
        clear_counters();
        free(array_copy);
    }
    print_table(data, times);

    get_sort_function(best_sort, &sort, name);
    printf("O melhor algoritmo de ordenação foi o %s\n", name);
    if (heuristic_sort == best_sort) {
        printf("O modo adaptativo, de fato, escolheu o melhor algoritmo!\n");
    } else {
        delta = heuristic_time - best_time;
        printf("O modo adaptativo NÃO escolheu o melhor algoritmo.\nA diferença causada por isso é de %lfs\n", delta);
    }
    for (int i = 0; i < 7; i++)
        free(data[i]);
    free(data);
    free(name);
    free(times);
}
