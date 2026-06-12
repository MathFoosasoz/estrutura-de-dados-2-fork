#include "headers/utils.h"
#include "headers/sorts_nlogn.h"
#include "headers/sorts_quadratic.h"
#include "headers/sorts_alternative.h"

/*--------------- Sort Method ---------------*/
// Pega a função pelo seu enum
void get_sort_function (SortMethod choice, Sort* sort, char* sort_name) {
    if (choice == SORT_RADIX) {
        *sort = bytewise_radix_sort;
        strcpy(sort_name, "RADIX SORT");
    } else if (choice == SORT_COUNT) {
        *sort = count_sort;
        strcpy(sort_name, "COUNTING SORT");
    } else if (choice == SORT_MERGE) {
        *sort = merge_sort;
        strcpy(sort_name, "MERGE SORT");
    } else if (choice == SORT_QUICK) {
        *sort = quick_sort;
        strcpy(sort_name, "QUICK SORT");
    } else if (choice == SORT_BUBBLE) {
        *sort = bubble_sort;
        strcpy(sort_name, "BUBBLE SORT");
    } else if (choice == SORT_SELECTION) {
        *sort = selection_sort;
        strcpy(sort_name, "SELECTION SORT");
    } else if (choice == SORT_INSERTION) {
        *sort = insertion_sort;
        strcpy(sort_name, "INSERTION SORT");
    } else if (choice == ALREADY_SORTED) {
        *sort = quick_sort; // Should not be used, but let's make it quick in case someone tries to break the code
        strcpy(sort_name, "ALREADY SORTED");
    } else if (choice == REVERSE_SORTED) {
        *sort = quick_sort; // Should not be used, but let's make it quick in case someone tries to break the code
        strcpy(sort_name, "REVERSE SORTED");
    } else {
        printf("wtf?!\nI think you did something wrong\n");
        exit(0);
    }
    return;
}

/*--------------- Counters ---------------*/
long long int ctr_compare = 0;
long long int ctr_assign = 0;
long long int ctr_recursion_depth = 0;
long long int ctr_recursion_call = 0;
long long int ctr_mem_alloc = 0;

/*--------------- Counter-related Functions ---------------*/
// Limpa os contadores
void clear_counters() {
    ctr_compare = 0;
    ctr_assign = 0;
    ctr_recursion_depth = 0;
    ctr_recursion_call = 0;
    ctr_mem_alloc = 0;
}

// Printa os contadores
void print_counters() {
    printf( "Comparisons         | %11lld\n"
            "Assignments         | %11lld\n"
            "Max Recursion Depth | %11lld\n"
            "Recursive Calls     | %11lld\n"
            "Memory Allocated    | %11lld\n",
            ctr_compare, ctr_assign, ctr_recursion_depth, ctr_recursion_call, ctr_mem_alloc
            );
}

// Printa os contadores e o tempo
void print_parameters (double dt) {
    printf( "Time taken          : %.8fs\n"
            "Comparisons         : %lld\n"
            "Assignments         : %lld\n"
            "Max Recursion Depth : %lld\n"
            "Recursive Calls     : %lld\n\n",
            dt, ctr_compare, ctr_assign, ctr_recursion_depth, ctr_recursion_call);
}

// Atribui um elemento ao outro e conta a operação
void assign (int* a, int b) {
    ctr_assign++;
    *a = b;
}

// Troca um elemento com outro e conta as operações
void swap (int* a,int *b) {
    int tmp;
    assign(&tmp, *a);
    assign(a, *b);
    assign(b, tmp);
}

// Compara (<) um elemento ao outro e conta a operação
int compare_l (int a, int b) {
    ctr_compare++;
    return a < b;
}

// Compara (<=) um elemento ao outro e conta a operação
int compare_le (int a, int b) {
    ctr_compare++;
    return a <= b;
}

// Compara (>) um elemento ao outro e conta a operação
int compare_g (int a, int b) {
    ctr_compare++;
    return a > b;
}

// Compara (>=) um elemento ao outro e conta a operação
int compare_ge (int a, int b) {
    ctr_compare++;
    return a >= b;
}

/*--------------- Simple facilitators ---------------*/

// Retorna o módulo do elemento
int abs (int a) {
    return compare_ge(a, 0) ? a: -a;
}

// Ordena 3 elementos
void sort_three(int* a, int* b, int *c) {
    if (compare_l(*b, *a))
        swap(b, a);
    if (compare_l(*c, *a))
        swap(c, a);
    if (compare_l(*c, *b))
        swap(c, b);
}

// Max e Min definidos como macro já no header

/*--------------- Bit-wise facilitators ---------------*/
// Pega o log2 de um número
int fast_log2 (unsigned int v) {
    int l = 0;
    int r = 32;
    while (l < r) {
        const int m = (l+r)/2;
        if (v & (1<<m)) {
            l = m + 1;
        } else 
            r = m;
    }
}

// Conta o número de bytes
int count_bytes (unsigned int v) {
    return (fast_log2(v) >> 3) + 1;
}

// Returna o n-ésimo byte de um número contando a partir do LsB 
unsigned char get_nth_byte (unsigned int v, unsigned char n) {
    return (v >> (n<<3)) & 0xFF;
}

/*--------------- Array-related functions ---------------*/
// Copia um array
int* copy_array (int* array, int size) {
    int* array2 = malloc(size*sizeof(int));
    for (int i = 0; i < size; i++)
        array2[i] = array[i];
    return array2;
}

// Printa um array
void print_array (int* array, int size) {
    for (int i = 0; i < size-1; i++)
        printf("%d ", array[i]);
    printf("%d\n", array[size-1]);
}

// Verifica a correta ordenação de um array
bool is_sorted (int* array, int size) {
    for (int i = 1; i < size; i++)
        if (array[i-1] > array[i]) {
            printf("Oops: %d > %d is false\n", array[i-1], array[i]);
            return false;
        }
    return true;
}
