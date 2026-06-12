#include "headers/sorts_nlogn.h"
#include "headers/utils.h"

/*--------------- Quick Sort ---------------*/

// Apenas uma função no mesmo padrão de parâmetros das outras
// para deixar o código mais limpo
void quick_sort (int* array, int n) {
	quick_sort_iteration(array, 0, n-1, 0);
}

// Chamada recursiva do Quick Sort em relação a cada
// array parcialmente ordenado em relação ao pivô
void quick_sort_iteration (int* array, int begin, int end, int depth) {
    // Contadores
    ctr_recursion_call++;
    if (depth > ctr_recursion_depth)
        ctr_recursion_depth = depth;
    // O sort em si
	if (begin < end) {
		int p = partition(array, begin, end);
		quick_sort_iteration(array, begin, p-1, depth+1);
		quick_sort_iteration(array, p+1, end, depth+1);
	}
}

// Divide array em:
// - Maior que pivô
// - Menor que pivô
int partition (int* array, int begin, int end) {
    // Escolhe pivô
    int pivot;
    // int index;
    // switch ((begin+end) % 3){
    // case 0:
    //     index = begin;
    //     break;
    // case 1:
    //     index = (begin+end)/2;
    //     break;
    // default:
    //     index = end;
    //     break;
    // }

	int mid = begin + (end - begin) / 2;

	sort_three(&array[begin],&array[mid],&array[end]);
	swap(&array[mid], &array[begin]);

	assign(&pivot, array[begin]); 
    // Define cursores
	int left = begin+1;
	int right = end;
    // Move cursores até achar um elemento errado em cada extremidade
    // Uma vez acha, os troca e procura os pŕoximos até que esteja separado em:
    // Menores que pivô | pivô | maiores que pivô
	while (left <= right) {
        while (left <= end && compare_l(array[left], pivot)) left++;
        while (right > begin && compare_l(pivot, array[right])) right--;
        if (left <= right) {
            swap(&array[left], &array[right]);
            left++;
            right--;
        }
    }
    // Reordena últimos valores e retorna posição do pivô
	assign(&array[begin], array[right]);
	assign(&array[right], pivot);
	return right;
}

/*--------------- Merge Sort ---------------*/

// Apenas uma função no mesmo padrão de parâmetros das outras
// para deixar o código mais limpo
void merge_sort (int* array, int n) {
	merge_sort_iteration(array, 0, n-1, 0);
}

// Divide recursivamente sobre suas metades e as junta depois
void merge_sort_iteration (int* array, int begin, int end, int depth) {
    // Contadores
    ctr_recursion_call++;
    if (depth > ctr_recursion_depth)
        ctr_recursion_depth = depth;
    // O sort em si
	if (begin < end) {
		int middle = (begin+end)/2;
		merge_sort_iteration(array, begin, middle, depth+1);
		merge_sort_iteration(array, middle+1, end, depth+1);
		if (array[middle] <= array[middle+1]) return;
		merge(array, begin, end);
	}
}

// Junta duas metades já ordenadas em um único array ordenado
// Para tal, requer um array temporário, o qual foi alocado
// estaticamente para maior eficiência
void merge (int* array, int begin, int end) {
	static int temp_array[10000000]; // Do better... (#define something)
	int middle = (begin+end)/2;
	int i = begin;
	int j = begin;
	int k = middle+1;

	while (j <= middle && k <= end) {
		if (compare_le(array[j], array[k])) {
			assign(&temp_array[i], array[j]);
			j++;
		} else {
			assign(&temp_array[i], array[k]);
			k++;
		}
		i++;
	}
	while (j <= middle) {
		assign(&temp_array[i], array[j]);
		i++;
		j++;
	}
	while (k <= end) {
		assign(&temp_array[i], array[k]);
		i++;
		k++;
	}
	for (i = begin; i <= end; i++)
		assign(&array[i], temp_array[i]);
}
