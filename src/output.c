#include "headers/output.h"

double test_sort(Sort sort, int* array, int size, bool kill_on_null) {
    struct timespec begin;
    struct timespec end;

    double dt;

    timespec_get(&begin, TIME_UTC);
    sort(array, size);
    timespec_get(&end, TIME_UTC);

    if (!sort_abort) {
        double dt = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9;

        if (!is_sorted(array, size)) {
            print_array(array, size);
            printf("\nAs you can see, it is not sorted...\nI'll interrupt it here just to get your attention\n");
            exit(0);
        }

        return dt;
    } else if (kill_on_null) {
        printf("O sort falhou em alocar memória. Abortando.\n");
        exit(0);
    }

    return -1;
}

// error_flags[x] : indica se o sort x foi executado sem erros

// times[x] : tempo de execução do sort x

// data[0][x] : comparações do sort x
// data[1][x] : atribuições do sort x
// data[2][x] : altura da árvore de recursão do sort x
// data[3][x] : número de nós na árvore de recursão do sort x
// data[4][x] : quantidade de INTs alocados

bool error_flags[7];
double times[7];
long long int data[6][7];

const char* metrics_col[] = {
    "Metric",
    "Time Taken",
    "Comparisons",
    "Assignments",
    "Max Recursion Depth",
    "Recursive Calls",
    "Memory Allocated",
    "Score"
};

const char *sorts_small[] = {
    "Radix",
    "Count",
    "Merge",
    "Quick",
    "Bubble",
    "Selection",
    "Insertion"
};

void print_table() {
    printf("%-20s", metrics_col[0]);

    for (int k = 0; k < 7; k++) {
        printf("| %11s ", sorts_small[k]);
    }

    printf("\n%-20s", metrics_col[1]);

    for (int k = 0; k < 7; k++) {
        if (!error_flags[k])
            printf("| %10lfs ", times[k]);
        else
            printf("| %11s ", "(ERRO)");
    }
    
    printf("\n");

    for (int j = 0; j < 5; j++) {
        printf("%-20s", metrics_col[j+2]);

        for (int k = 0; k < 7; k++) {
            if (!error_flags[k])
                printf("| %11lld ", data[j][k]);
            else
                printf("| %11s ", "(ERRO)");
        }

        printf("\n");
    }

    printf("%-20s", metrics_col[7]);

    for (int k = 0; k < 7; k++) {
        switch (k) {
        case SORT_RADIX:
        case SORT_COUNT:
        case SORT_MERGE:
        case SORT_QUICK:
            printf("| %11lld ", data[5][k]);
            break;
        
        default:
            printf("| %11s ", "----");
            break;
        }
    }

    printf("\n");

}

void write_to_file(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fprintf(file, "%s", metrics_col[0]);

    for (int k = 0; k < 7; k++) {
        fprintf(file, ",%s", sorts_names[k]);
    }

    fprintf(file, "\n%s", metrics_col[1]);

    for (int k = 0; k < 7; k++) {
        if (!error_flags[k])
            fprintf(file, ",%.15lf", times[k]);
        else
            fprintf(file, ",(ERRO)");
    }

    for (int j = 0; j < 6; j++) {
        fprintf(
            file,
            "\n"
            "%s", metrics_col[j+2]
        );

        for (int k = 0; k < 7; k++) {
            if (!error_flags[k] || j == 6)
                fprintf(file, ",%lld", data[j][k]);
            else
                fprintf(file, ",(ERRO)");
        }
    }

    fclose(file);
}


void benchmark (int* array, int size, bool output_to_file, bool kill_on_null) {
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
    long long scores[4];
    SortMethod heuristic_sort = choose_sort(array, size, scores), best_sort = SORT_RADIX;
    get_sort_function(heuristic_sort, &sort, name);
 
    printf(
        "Scores determinados pela heurística:\n"
        "Radix sort: %11lld\n"
        "Count sort: %11lld\n"
        "Merge sort: %11lld\n"
        "Quick sort: %11lld\n"
        "Algoritmo escolhido pela heurística (mínimo score): %s\n",
        scores[0], scores[1], scores[2], scores[3], name
    );

    data[5][0] = scores[0];
    data[5][1] = scores[1];
    data[5][2] = scores[2];
    data[5][3] = scores[3];
    data[5][4] = 0;
    data[5][5] = 0;
    data[5][6] = 0;

    // Collect data
    double dt, delta, heuristic_time, best_time = -1;
    int* array_copy;
    for (int i = 0; i < 7; i++) {
        SortMethod sort_name;
        array_copy = copy_array(array, size);
        
        get_sort_function(i, &sort, NULL);
        // Store data
        dt = test_sort(sort, array_copy, size, false);

        if (sort_abort) {
            if (kill_on_null) {
                printf("O sort %s falhou em alocar memória. Abortando.\n", sorts_names[i]);
                exit(0);
            }

            error_flags[i] = true;
        
            clear_counters();
            free(array_copy);
        
            continue;
        } else {
            error_flags[i] = false;
        }

        times[i]   = dt;
        data[0][i] = ctr_compare;
        data[1][i] = ctr_assign;
        data[2][i] = ctr_recursion_depth;
        data[3][i] = ctr_recursion_call; 
        data[4][i] = ctr_mem_alloc;
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
    print_table();

    if(output_to_file){
        write_to_file("output.csv");
        printf("Output escrito para output.csv!\n");

        // escolha heurística

        FILE* file = fopen("escolha_heuristica.txt", "wt");
        if (file != NULL) {
            fprintf(file, "%s", sorts_names[heuristic_sort]);
            fclose(file);
        }
    }

    printf("O melhor algoritmo de ordenação foi o %s\n", sorts_names[best_sort]);
    if (heuristic_sort == best_sort) {
        printf("O modo adaptativo, de fato, escolheu o melhor algoritmo!\n");
    } else {
        delta = heuristic_time - best_time;
        printf("O modo adaptativo NÃO escolheu o melhor algoritmo.\nA diferença causada por isso é de %lfs\n", delta);
    }

    free(name);
}
