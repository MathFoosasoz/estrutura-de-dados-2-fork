#include "headers/sorts_alternative.h"
#include "headers/utils.h"

/*--------------- Count Sort ---------------*/

// o count sort também pode ser feito com floats, partindo-se do mesmo princípio de interpretar os bytes de um float como um inteiro.
// ele é, no entanto, extremamente ineficiente nesse caso, já que os bytes de um float, embora ordenados, saltam muito mais do que de um inteiro.

// execução: O(n + k), onde k = (max(array) - min(array))
// memória auxiliar: O(k)
void count_sort (int* array, const int n) {
    int smallest;
    int largest;
    assign(&smallest, array[0]);
    assign(&largest, array[0]);
    for (int i = 0; i < n; ++i) {
        assign(&smallest, min(smallest, array[i]));
        assign(&largest, max(largest, array[i]));
    }
    // normalmente, o count sort tem um problema que você não pode tratar números negativos, já que eles não são indexáveis
    // nesse caso, basta fazermos ao invés de smallest para largest, indexar de 0 até largest-smallest

    // então, ao invés de indexar os valores como bucket[array[i]], que tem index i e [smallest,largest],
    // se indexa através de bucket[abs(array[i] - smallest)], que tem index 0 e abs(largest-smallest)
    const int bucket_size = abs(largest - smallest) + 1;
    int* bucket = (int*) calloc(bucket_size, sizeof(int));
    ctr_mem_alloc += bucket_size;
    if (bucket == NULL) {
        printf("Error allocating auxiliary array on Count Sort\nIs it too big or just random error?\n");
        exit(0);
    }
    for (int i = 0; i < n; ++i) {
        int normalized_index = abs(array[i] - smallest);
        // bucket é 0-inicializado pelo calloc
        bucket[normalized_index]++;
    }
    int idx = 0;
    for (int i = 0; i < bucket_size; ++i) {
        int count = bucket[i];
        while (count--) {
            assign(&array[idx++], i + smallest);
        }
    }
    free(bucket);
}

/*--------------- Bytewise Radix Sort ---------------*/


int radix_bucket[256];


void bytewise_radix_sort(int* array, const int n){
    ctr_mem_alloc += 256; // Bucket
    internal_bytewise_radix_sort(array,n,0);
}

// curiosidade: esse radix sort funciona com floats também. isso é porque os bytes do float são bem ordenados, ou seja, se ordenarmos os bytes, ordenamos os números também. isso acontece sem mudança de performance.

// fazemos radix sort byte a byte já que isso permite que trabalhemos somente com log2 e operações bitwise
// que são, na prática, O(1)

// execução: O(n)
// memória auxiliar: O(n)

void internal_bytewise_radix_sort (int* array, const int n, const unsigned char skippable_bytes) {
    int* tmp = calloc(n, sizeof(int));
    ctr_mem_alloc += n;

    // radix sort tem o problema que ele não sabe discernir números negativos de positivos.
    // se tratarmos o bit de sinal como um bit de valor (ex: -3 = (1) 11 = 111 = 5), teremos problema de ordenação, já que todo
    // número negativo terá o primeiro bit setado, fazendo que ele seja maior que os positivos.
    // para isso, invertemos o primeiro bit de cada número. dessa forma, um número positivo sempre será maior que um número negativo, e a comparação de magnitude ocorre normalmente.   
    for (int i = 0; i < n; ++i) {
        array[i] ^= 0x80000000;
    }

    for (unsigned char byte_idx = 0; byte_idx < 4 - skippable_bytes; ++byte_idx) {
        memset(radix_bucket, 0, 256 * sizeof(int)); // Desprezado nos counters
        for (int i = 0; i < n; ++i) {
            unsigned char current_byte = get_nth_byte(array[i], byte_idx);
            radix_bucket[current_byte]++;
        }
        for (int i = 1; i < 256; ++i) {
            radix_bucket[i] += radix_bucket[i-1];
        }
        for (int i = n-1; i >= 0; --i) {
            unsigned char current_byte = get_nth_byte(array[i], byte_idx);
            assign(&tmp[--radix_bucket[current_byte]], array[i]);
        }
        memcpy(array, tmp, n * sizeof(int)); // Desprezado nos counters
    }

    // isso desfaz a inversão que fizemos no começo
    for (int i = 0; i < n; ++i) {
        array[i] ^= 0x80000000;
    }
    free(tmp);
}
