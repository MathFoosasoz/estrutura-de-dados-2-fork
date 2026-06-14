#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include "headers/generate.h"
#include "headers/write.h" 

#define COUNT_MAX 1000000000

int sizes [] = { 4, 8, 16, 32, 64,
     128, 256, 512, 1024, 1 << 11, 
    1 << 12, 1<<13, 1<<14,  1<<15,
    1<<16, 1<<17};

static void swap(int *a, int *b) {
    int tmp = *a; *a = *b; *b = tmp;
}

void make_random_small_range(void) {
    printf("Gerando: Aleatório com range pequeno\n");
    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
            return;
        }
        for (int i = 0; i < n; i++)
            arr[i] = rand() % n;
        write_array("casos_random_small_range", arr, n);
        free(arr);
    }
}

void make_random_full_range(void) {
    printf("Gerando: Aleatório com range grande\n");
    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
            return;
        }
        for (int i = 0; i < n; i++)
            arr[i] = (rand());
        write_array("casos_random_full_range", arr, n);
        free(arr);
    }
}

void make_almost_sorted(void) {
    printf("Gerando: Quase ordenado\n");
        for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
            return;
        }
        int start = rand() % 100;
        for (int i = 0; i < n; i++)
        arr[i] = (start + i);
            int swaps = (n * 5) / 100;
        if (swaps < 1) swaps = 1;
        for (int i = 0; i < swaps; i++) {
            int a = rand() % n;
            int b = rand() % n;
            swap(&arr[a], &arr[b]);
        }
        write_array("casos_almost_sorted", arr, n);
        free(arr);
    }
}

void make_almost_rev_sorted(void) {
    printf("Gerando: Quase ordenado reverso\n");
        for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
                        return;
        }
        int start = n + (rand() % 1000);
        for (int i = 0; i < n; i++)
            arr[i] = (start - i);
            int swaps = (n * 5) / 100;
            if (swaps < 1) swaps = 1;
            for (int i = 0; i < swaps; i++) {
            int a = rand() % n;
            int b = rand() % n;
            swap(&arr[a], &arr[b]);
        }
        write_array("casos_almost_reverse_sorted", arr, n);
        free(arr);
    }
}

void make_fully_reversed(void) {
    printf("Gerando: Ordenado reverso\n");
        for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
                        return;
        }
        int start = n + (rand() % 1000);
        for (int i = 0; i < n; i++){
        arr[i] = (start - i);
        }
        write_array("casos_fully_reversed", arr, n);
        free(arr);
    }
}

void make_repetition(void) {
    printf("Gerando: 50 por cento repetição\n");
    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
                        return;
        }
        for (int i = 0; i < n; i++)
        arr[i] = (rand() % (n / 2));
        write_array("casos_repetition_50pct", arr, n);
        free(arr);
    }
}

void make_antimerge(void) {
    printf("Gerando: anti merge sort\n");
    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
            return;
        } 
        int half = n / 2;
        for (int i = 0; i < half; i++)
            arr[i] = n - 2 * i;      
        for (int i = 0; i < n - half; i++)
            arr[half + i] = n - 1 - 2 * i; 

        write_array("casos_antimerge", arr, n);
        free(arr);
    }
}

void make_antiquick(void) {
    printf("Gerando: anti quick sort\n");
        // Organ pipe para forçar escolha ruim de pivo
    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
            return;
        } 
        for (int i = 0; i < n / 2; i++)  
        arr[i]      = i * 2;

        for (int i = n / 2; i < n; i++)  
            arr[i]      = (n - 1 - i) * 2 + 1;
        write_array("casos_antiquick", arr, n);
        free(arr);
    }
}

void make_anticount(void) {
    printf("Gerando: anti counting sort\n");
    for (int s = 0; s < sizeof(sizes) / sizeof(sizes[0]); s++) {
        int n = sizes[s];
        int *arr = malloc(n * sizeof(int));
        if (arr == NULL) {
            fprintf(stderr, "Erro na alocação de memória!\n");
            return;
        }

        arr[0] = - COUNT_MAX;
        arr[n-1] = COUNT_MAX;
        for (int i = 1; i < n - 1; i++)
            arr[i] = rand() % (COUNT_MAX / 2);
        for (int i = n - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            swap(&arr[i], &arr[j]);
        }
        write_array("casos_anticount", arr, n);
        free(arr);
    }
}