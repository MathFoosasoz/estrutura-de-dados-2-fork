#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*--------------- Sort funcion type ---------------*/
typedef void (*Sort)(int*, int);  // Tipo de uma função de ordenação

/*--------------- Sort Method ---------------*/
// Used by generic_sort and main files
#ifndef SORT_METHOD
#define SORT_METHOD

typedef enum {
    ADAPTATIVO = -2,     // Escolhe um pela heurística
    BENCHMARK = -1,      // Roda todos e compara
    SORT_RADIX = 0,      // Alternativo
    SORT_COUNT = 1,      // Alternativo
    SORT_MERGE = 2,      // nlogn
    SORT_QUICK = 3,      // nlogn
    SORT_BUBBLE = 4,     // n^2
    SORT_SELECTION = 5,  // n^2
    SORT_INSERTION = 6,  // n^2
    ALREADY_SORTED = 7,  // Caso, quando definindo as métricas, assim se perceba
    REVERSE_SORTED = 8   // Caso, quando definindo as métricas, assim se perceba
} SortMethod;

#endif

void get_sort_function (SortMethod choice, Sort* sort, char* sort_name); // Pega a função pelo seu enum

/*--------------- Counters ---------------*/
extern long long int ctr_compare;            // Contador de comparações
extern long long int ctr_assign;             // Contador de atribuções
extern long long int ctr_recursion_depth;    // Contador da profundidade da recursão
extern long long int ctr_recursion_call;     // Contador do número de chamadas recursivas
extern long long int ctr_mem_alloc;          // Contador de memória alocada

/*--------------- Counter-related Functions ---------------*/
void clear_counters();                       // Limpa os contadores
void print_counters();                       // Printa os contadores
void print_parameters (double dt);           // Printa os contadores e o tempo
void assign (int* a, int b);                 // Atribui um elemento ao outro e conta a operação
void swap (int* a,int *b);                   // Troca um elemento com outro e conta as operações
int compare_l (int a, int b);                // Compara (<) um elemento ao outro e conta a operação
int compare_le (int a, int b);               // Compara (<=) um elemento ao outro e conta a operação
int compare_g (int a, int b);                // Compara (>) um elemento ao outro e conta a operação
int compare_ge (int a, int b);               // Compara (>=) um elemento ao outro e conta a operação

/*--------------- Simple facilitators ---------------*/
int abs (int a);                             // Retorna o módulo do elemento
void sort_three (int* a, int* b, int *c);    // Ordena 3 elementos

#define min(a,b) (compare_l((a), (b)) ? (a) : (b))
#define max(a,b) (compare_g((a), (b)) ? (a) : (b))

/*--------------- Bit-wise facilitators ---------------*/
int fast_log2 (unsigned int v);              // Pega o log2 de um número
int count_bytes (unsigned int v);            // Conta o número de bytes
unsigned char get_nth_byte (unsigned int v, unsigned char n); // Retorna o n-ésimo byte de um unsigned int, começando pelo lsB

/*--------------- Array-related functions ---------------*/
int* copy_array (int* array, int size);      // Copia um array
void print_array (int* array, int size);     // Printa um array
bool is_sorted (int* array, int size);       // Verifica a correta ordenação de um array
