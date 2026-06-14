#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "headers/write.h"
#include "headers/generate.h"
#include <limits.h>
#include <time.h>

typedef struct {
    const char *nome;
    void (*fn)(void);
} TestCase;

TestCase cases[] = {
    { "Aleatório com range pequeno",        make_random_small_range },
    { "Aleatório com range grande",         make_random_full_range  },
    { "Quase ordenado",             make_almost_sorted      },
    { "Quase ordenado reverso",     make_almost_rev_sorted  },
    { "Ordenado reverso",            make_fully_reversed     },
    { "50 por cento repetição",      make_repetition  },
    { "Anti merge sort",           make_antimerge          },
    { "Anti quick sort",           make_antiquick          },
    { "Anti counting sort",        make_anticount          },
};

int n_cases = sizeof(cases) / sizeof(cases[0]);

int main(void) {
    srand((unsigned)time(NULL));

    printf("Quais casos deseja gerar?\n");
    for (int i = 0; i < n_cases; i++)
        printf("  %d. %s\n", i + 1, cases[i].nome);
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > n_cases) {
        printf("Opcao invalida.\n");
        return 1;
    }

    cases[choice - 1].fn();
    printf("Pressione Enter para sair\n");
    getchar(); 
    getchar(); 
    return 0;
}
