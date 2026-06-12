#include "headers/sorts_nlogn.h"
#include "headers/sorts_quadratic.h"
#include "headers/sorts_alternative.h"
#include "headers/generic_sort.h"

void get_metrics (const int* array, const int n, Metrics* met) {
    met->size = n;
    
    int smallest, largest;
    smallest = largest = array[n-1];
    int rep_count, inv_count;
    unsigned int bit_accumulator = array[n-1];
    rep_count = inv_count = 0;

    float mean = array[n-1];

    for (unsigned int i = 0; i < n-1; ++i) {
        smallest = min(smallest, array[i]);
        largest = max(largest, array[i]);
        inv_count += array[i] > array[i+1];   
        bit_accumulator |= array[i];   
        mean += array[i];
    }
    mean /= n;
    
    met->direct_inversion_count = inv_count;
    met->smallest = smallest;
    met->largest = largest;
    met->highest_bit = bit_accumulator;
    met->size = n;
    met->mean = mean;
}

// Os comentários nesse arquivo são o raciocínio para chegar na fórmula que dá o score de cada uma das funções (não servem como documentação)


// Contagem de operações no radix:
// 2n assigns (inverter/desinverter o primeiro bit)
/* k iterações do loop que fazem:
    2n iterações de get_nth_byte
    256 somas
    2n cópias 
*/
// no total, 2n + k (2n + 2n + 256) = 2n(1 + 2k) + 256k

long long radix_score (const Metrics* met) {
    unsigned int k = fast_log2(met->highest_bit) / 8 + 1;
    int n = met->size;
    
    float n_arithmetic = k * (2.0f * n + 256.0f);
    float n_assign = 2.0f * n + k * 2.0f * n; 
    float n_allocations = n; 
    
    return (long long)(n_arithmetic * WEIGHT_ARITHMETIC + n_assign * WEIGHT_ASSIGN + n_allocations * WEIGHT_ALLOCATION);
}

// Operações do count sort:
// 2n max/min, que fazem uma comparação e um assign, 4n
// alocação de k memória.
// n subtrações e módulos. Módulo tem uma negação e uma comparação. 3n 
// Por fim, k operações.
// No total, 7n + 2k

long long count_score (const Metrics* met) {
    int k = met->largest - met->smallest;
    int n = met->size;
    
    float n_arithmetic = 5.0f * n; 
    float n_assign = 2.0f * n + k;
    float n_allocations = k; 
    
    return (long long)(n_arithmetic * WEIGHT_ARITHMETIC + n_assign * WEIGHT_ASSIGN + n_allocations * WEIGHT_ALLOCATION);
}

// Operações do merge sort:
// Alocação inicial de n elementos
// Profundidade da árvore: log(n), com, em um k-ésimo andar, 2^k elementos
// Calcularemos por andar, e depois somamos tudo.
// temos begin - end = k (tamanho do array local). fazemos, então
// k * (1 comparação + 1 assign) + k/2 * 1 assign + k/2 * 1 assign + k assigns
// no total, temos 2k + k/2 + k/2 = 3k por andar
// nosso uso final será f(n) = 3 * somatória(i=0,log(n),2^i) = 3n*log(n)
// vale notar que se uma das metades já estiver ordenada, o merge sort consegue pular um passo.
// a gente estima a probabilidade disso ocorrer ao comperar o número de inversões estimado com o número de inversões que de fato temos.
// fazemos isso por andar. a chance é baixa, mas se conseguimos cortar em um andar, a gente elimina o trabalho de todos os filhos
// em um dado nível k, teremos uma probabilidade de (1-p)^2^(k-1) disso ocorrer, onde p = a probabilidade de dois números estarem inversos
// então, o trabalho nesse andar k será a probabilidade de ser NÃO ser cortado multiplicado por 3k
// infelizmente, isso tira a nossa recorrência. Teremos então que essa função de telemetria é O(log(n))

long long merge_score (const Metrics* met) {
    int n = met -> size;
    
    float p = (float) met->direct_inversion_count / max(n-1.0f,1.0f);

    if (p >= 0.5) {
        float n_arithmetic = n + n * fast_log2(n);
        float n_assign = 2.0f * n * fast_log2(n);
        float n_allocations = n;
        return (long long)(n_arithmetic * WEIGHT_ARITHMETIC + n_assign * WEIGHT_ASSIGN + n_allocations * WEIGHT_ALLOCATION);
    }

    float work_arithmetic = 0.0f;
    float work_assigns = 0.0f;
    double probability_of_cut = 1.0f-p;
    int levels = fast_log2(n);

    for (int k = 1; k <= levels; ++k) {
        float execution_probability = 1.0f - probability_of_cut;
        work_arithmetic += n * execution_probability;
        work_assigns += 2.0f * n * execution_probability;
        probability_of_cut *= probability_of_cut;
    }
    
    float n_arithmetic = n + work_arithmetic;
    float n_assign = work_assigns;
    float n_allocations = n;
    
    return (long long)(n_arithmetic * WEIGHT_ARITHMETIC + n_assign * WEIGHT_ASSIGN + n_allocations * WEIGHT_ALLOCATION);
}


// Operações do quick sort:
// Vamos fazer inicialmente uma análise assumindo um array perfeitamente aleatório, e depois aplicar os parâmetros que temos.
// Profundidade da árvore de log(n) 
// No k-ésimo andar, com k = right-left, a gente terá um loop que:
/*
    enquanto left < right, ou seja, k/5 vezes:
    - fazer 2 comparações
    - fazer 1 soma
    - fazer uma troca quando left for menor que right. isso ocorre com uma probabilidade de n/5 com uma distribuição de median of three. ou seja, 1/5 de um swap, ou 3 / 5 assigns. e 1/5 de comparação
    - Um último check  k/5

*/
// no total então, temos k(3 + 3/5 + 2/5 + 4/5) = 24k/5 operações, que ocorrerão log(n) vezes.
// ou seja, em um caso aleatório, podemos esperar 24n/5 log(n) operações.
// Vale ressaltar que seria muito menos complexo analisar esse algoritmo se ele tivesse um pivô simples (como ar[0])
// Em um caso não aleatório, temos alguns fatores que mudam.
// A gente passa agora a considerar uma desordem de inversões, como no merge sort
// E estimamos o número de duplicatas (que são ruins para o quick sort). A gente consegue estimar isso ao ver a nossa range max - min. Por exemplo, se a range < n, sempre teremos no mínimo uma duplicata.
// Temos então um número de repetições estimados de n - range, ou, 1 - range/n como taxa

long long quick_sort_score (const Metrics* met) {
    int n = met->size;

    float expected_inversions = (n-1)/2.0f;
    float inversion_disorder = met->direct_inversion_count / expected_inversions;
    inversion_disorder = min(1.0f,inversion_disorder);

    float swap_by_disorder = n / 5.0f * inversion_disorder;

    int range = met->largest - met->smallest;
    float swap_by_duplicate = n / 2.0f * max(1.0f - (float) range/n,0.0f);

    float effective_swaps = swap_by_disorder > swap_by_duplicate ? swap_by_disorder : swap_by_duplicate;
    
    float n_arithmetic = fast_log2(n) * (3.0f * n + effective_swaps * 4.4f);
    float n_assign = fast_log2(n) * (effective_swaps * 3.0f);
    float n_allocations = 0;
    
    return (long long)(n_arithmetic * WEIGHT_ARITHMETIC + n_assign * WEIGHT_ASSIGN + n_allocations * WEIGHT_ALLOCATION); 
}

// Bubble sort nunca é considerado pois ele é estritamente pior do que os outros métodos, em quase todos os casos.

long long bubble_sort_score (const Metrics* met){
    const int n = met->size;

}

// No caso do insertion sort, ele não é considerado pois precisa saber do número inversões no array (a complexidade é, na prática, O(n * numero_de_inversoes))
// Mas o algoritmo para contar inversões é essencialmente um merge sort O(n log n). Ou seja, faria mais sentido ordenar a lista
// Podemos pegar uma amostra constante (100 elementos, 1000 elementos, etc) mas isso introduz um erro na função ao passo que ele cresce.
// E não podemos esquecer que insertion sort é O(n^2).

void best_sort (int* array, const int n) {
    time_t begin, end;
    double dt, tot;
    printf("---------- MODO ADAPTATIVO ----------\nTamanho do array: %d\n", n);
    // Lida com casos simples
    if (n == 3) {
        sort_three(&array[0],&array[1],&array[2]);
        printf("Caso trivial, apenas ordenei os 3\nContadores:\n");
        print_counters();
        return;
    }

    if (n < 15) {
        begin = clock();
        insertion_sort(array,n);
        end = clock();
        dt = (end - begin) / (double) CLOCKS_PER_SEC;
        printf( "Algoritmo escolhido: INSERTION SORT\n"
                "Tempo: %10lfs\n", dt);
        print_counters();
        return;
    }

    // Coleta métricas
    Metrics met;
    begin = clock();
    get_metrics(array, n, &met);
    end = clock();
    dt = (end - begin) / (double) CLOCKS_PER_SEC;
    tot = dt;
    printf("Tempo coletando métricas : %10lfs\n", dt);

    if (met.direct_inversion_count == 0) {
        printf("Caso trivial, o vetor já está ordenado\n");
        return;
    }

    // Caso reversamente ordenado, já resolve
    if (met.direct_inversion_count == n-1) {
        printf("Reversamente ordenado? Trivial...\n");
        begin = clock();
        for (int i = n - 1; i >= n/2; --i) {
            swap(&array[i],&array[n-i-1]);
        }
        end = clock();
        dt = (end - begin) / (double) CLOCKS_PER_SEC;
        printf("Tempo: %10lfs\nContadores:\n", dt);
        print_counters();
        return; 
    }

    // Pega scores de cada sort
    begin = clock();
    long long scores[4]; 
    scores[SORT_RADIX] = radix_score(&met);
    scores[SORT_COUNT] = count_score(&met);
    scores[SORT_MERGE] = merge_score(&met);
    scores[SORT_QUICK] = quick_sort_score(&met);
    end = clock();
    dt = (end - begin) / (double) CLOCKS_PER_SEC;
    tot += dt;
    printf("Tempo calculando scores  : %10lfs\n", dt);

    SortMethod best_method = SORT_RADIX;
    long long min_score = scores[SORT_RADIX];

    // Vê qual foi o melhor
    for (int i = 1; i < 4; ++i) {
        if (scores[i] < min_score) {
            min_score = scores[i];
            best_method = (SortMethod)i;
        }
    }

    // Ordena com o sort escolhido
    begin = clock();
    switch (best_method) {
        case SORT_RADIX:
            // Radix Sort, mas levemente otimizado
            internal_bytewise_radix_sort(array, n, 5 - fast_log2(met.highest_bit) / 8 ); 
            end = clock();
            printf("Algoritmo escolhido: RADIX SORT\n");
            break;
        case SORT_COUNT:
            count_sort(array, n); 
            end = clock();
            printf("Algoritmo escolhido: COUNT SORT\n");
            break;
        case SORT_MERGE:
            merge_sort(array, n); 
            end = clock();
            printf("Algoritmo escolhido: MERGE SORT\n");
            break;
        case SORT_QUICK:
            quick_sort(array, n); 
            end = clock();
            printf("Algoritmo escolhido: QUICK SORT\n");
            break;
    }
    dt = (end - begin) / (double) CLOCKS_PER_SEC;
    tot += dt;
    printf( "Tempo de ordenação       : %10lfs\n"
            "---------- Métricas totais ----------\n"
            "Time Taken               : %10lfs\n"
            "Contadores:\n",
            dt, tot);
    print_counters();
}

// "Cópia" da função anterior que retorna apenas o nome da função que seria escolhida
// Usado para o benchmark

SortMethod choose_sort (int* array, const int n, long long scores[]) {
    // Coleta métricas
    Metrics met;
    get_metrics(array, n, &met);

    if (met.direct_inversion_count == 0) {
        return ALREADY_SORTED;
    }
    // Caso reversamente ordenado, já resolve
    if (met.direct_inversion_count == n-1) {
       return REVERSE_SORTED; 
    }

    // Pega scores de cada sort
    scores[SORT_RADIX] = radix_score(&met);
    scores[SORT_COUNT] = count_score(&met);
    scores[SORT_MERGE] = merge_score(&met);
    scores[SORT_QUICK] = quick_sort_score(&met);

    SortMethod best_method = SORT_RADIX;
    long long min_score = scores[SORT_RADIX];

    // Vê qual foi o melhor
    for (int i = 1; i < 4; ++i) {
        if (scores[i] < min_score) {
            min_score = scores[i];
            best_method = (SortMethod)i;
        }
    }

    return best_method;
}
