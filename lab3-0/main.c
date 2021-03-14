#include <stdio.h>
#include <malloc.h>

void Swap(int* first, int* second) {
    int first_copy = *first;
    *first = *second;
    *second = first_copy;
}

// Turns part of array in range [root; heap_bound] into heap
void HeapifyArray(int* array, int heap_bound, int root) {
    if(root >= heap_bound) {
        return;
    }

    int largest = root;
    int left = root * 2 + 1;
    int right = root * 2 + 2;

    if (left < heap_bound && array[left] > array[largest]) {
        largest = left;
    }

    if (right < heap_bound && array[right] > array[largest]) {
        largest = right;
    }

    if (largest != root) {
        Swap(&array[root], &array[largest]);
        HeapifyArray(array, heap_bound, largest);
    }

}

// Sorts array using Heap Sort
void SortArray(int* array, int arr_len) {

    // Turns whole array to heap
    for (int i = arr_len / 2 + 1; i >= 0; --i) {
        HeapifyArray(array, arr_len, i);
    }

    // Moves largest element (root because it's heap) upper
    // Heapify array again
    for (int i = arr_len - 1; i > 0; --i) {
        Swap(&array[0], &array[i]);
        HeapifyArray(array, i, 0);
    }

}

int main() {

    int arr_len;
    if (scanf("%d", &arr_len) != 1) {
        return 0;
    }

    int* array = malloc((int)sizeof(*array) * arr_len);

    for (int i = 0; i < arr_len; ++i) {
        if (scanf("%d", &array[i]) != 1) {
            return 0;
        }
    }

    SortArray(array, arr_len);

    for (int i = 0; i < arr_len; ++i) {
        printf("%d ", array[i]);
    }

    free(array);

}
