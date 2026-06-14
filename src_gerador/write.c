#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include "headers/write.h"
#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
#endif

void ensure_dir(const char *path) {
#ifdef _WIN32
    _mkdir(path);
#else
    mkdir(path, 0777);
#endif
}

void write_array(const char *folder, int *arr, int n) {
    ensure_dir(folder);

    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s/%d.txt", folder, n);

    FILE *f = fopen(filepath, "w");
    if (!f) {
        perror("Erro ao abrir arquivo\n");
        return;
    }

    for (int i = 0; i < n; i++)
        fprintf(f, "%d\n", arr[i]);

    fclose(f);
    printf("  Escrito: %s\n", filepath);
}
