#include "headers/sorts_nlogn.h"
#include "headers/sorts_quadratic.h"
#include "headers/sorts_alternative.h"
#include "headers/generic_sort.h"
#include "headers/utils.h"
#include "headers/input.h"
#include "headers/output.h"

int main (int argc, char** argv) {
    // Variáveis
    int* array;
    int size = 0;
    char* input = NULL; // PATH do arquivo de entrada
    SortMethod mode = ADAPTATIVO;

    // Determina operação de acordo com as flags
    deal_with_flags(argc, argv, &input, &mode, &size);

    // Lê array
    if (input == NULL || strcmp(input, "-") == 0)
        array = read_array(&size);
    else
        array = read_file(input, &size);

    if (mode == BENCHMARK) {
        benchmark(array, size);
    } else if (mode == ADAPTATIVO) {
        best_sort(array, size);
    } else {
        Sort sort;
        char* name = (char*)calloc(32, sizeof(char));
        get_sort_function(mode, &sort, name);

        printf("%s\n", name);
        double dt = test_sort(sort, array, size);

        print_parameters(dt);
        clear_counters();

        free(name);
    }
    free(array);
}
