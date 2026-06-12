#include <string.h>
#include <stdio.h>

void deal_with_flags (int argc, char** argv, char** input, int* mode, int* size); // Lida com as flags da linha de comando
int* read_file (char* input, int* size);           // Lê array de um arquivo
int* read_array (int* size);                       // Lê array do STDIN
int count_lines (FILE* file);                      // Conta as linhas no arquivo
void to_lowercase (char* str);                     // Transforma alfabeto maiúsculo em minúsculo
