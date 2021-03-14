#include <stdio.h>
#include <malloc.h>

void Swap(int* a, int* b) {
    int c = *b;
    *b = *a;
    *a = c;
}

// Divides array part [from:to] on 2 sides (less than some value and more than some value)
int DivideArray(int* array, int from, int to) {
    int middle = (from + to) / 2;
    int value = array[middle];

    int left = from;
    int right = to;

    while (left <= right) {
        while (array[left] < value) {
            ++left;
        }
        while (array[right] > value) {
            --right;
        }

        if (left >= right) {
            break;
        }

        Swap(&array[left], &array[right]);
        ++left;
        --right;
    }
    return right;
}

void SortArray(int * array, int from, int to) {
    if (from >= to) {
        return;
    }

    int middle = DivideArray(array, from, to);
    SortArray(array, from, middle);
    SortArray(array, middle + 1, to);
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

    SortArray(array, 0, arr_len - 1);

    for (int i = 0; i < arr_len; ++i) {
        printf("%d ", array[i]);
    }

    free(array);

}

