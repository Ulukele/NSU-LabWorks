#include "PriorityQueue.h"
#include <malloc.h>
#include <assert.h>
#include <stdbool.h>

static void Swap(short* first, short* second) {
    short holder = *first;
    *first = *second;
    *second = holder;
}

static void SwapElements(TPQueue* pQueue, short first, short second) {
    short* binaryHeap = pQueue->BinaryHeap;
    short* order = pQueue->Order;
    short firstValue = binaryHeap[first];
    short secondValue = binaryHeap[second];
    Swap(&order[firstValue], &order[secondValue]);
    Swap(&binaryHeap[first], &binaryHeap[second]);
}

bool CompareKeys(short first, short second, const int* keys) {
    return ((keys[second] == PQUEUE_INF_KEY && keys[first] != keys[second]) || keys[first] < keys[second]);
}

TPQueue* CreateEmptyPQueue(int maxLen, int* keys, short* order) {
    short* binaryHeap = malloc(sizeof(*binaryHeap) * maxLen);
    TPQueue* pQueue = malloc(sizeof(*pQueue));
    if (binaryHeap == NULL || pQueue == NULL) {
        free(binaryHeap);
        free(pQueue);
        return NULL;
    }
    pQueue->BinaryHeap = binaryHeap;
    pQueue->Keys = keys;
    pQueue->Order = order;
    pQueue->MaxLen = maxLen;
    pQueue->Len = 0;
    return pQueue;
}

void DeletePQueue(TPQueue* pQueue) {
    if (pQueue) {
        free(pQueue->BinaryHeap);
        free(pQueue);
    }
}

static void ShiftDown(TPQueue* pQueue, int idx) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    short* binaryHeap = pQueue->BinaryHeap;

    while (2 * idx + 1 < len) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int minimum = left;
        if (right < len && CompareKeys(binaryHeap[right], binaryHeap[left], pQueue->Keys)) {
            minimum = right;
        }
        if (!CompareKeys(binaryHeap[minimum], binaryHeap[idx], pQueue->Keys)) {
            break;
        }
        SwapElements(pQueue, idx, minimum);
        idx = minimum;
    }
}

static void ShiftUp(TPQueue* pQueue, int idx) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    short* binaryHeap = pQueue->BinaryHeap;

    int parent = (idx - 1) / 2;
    while (CompareKeys(binaryHeap[idx], binaryHeap[parent], pQueue->Keys)) {
        SwapElements(pQueue, idx, parent);
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

bool Enqueue(TPQueue* pQueue, short value) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    short* binaryHeap = pQueue->BinaryHeap;
    short* order = pQueue->Order;
    if (len < maxLen) {
        int idx = len;
        pQueue->Len = len + 1;
        binaryHeap[idx] = value;
        order[value] = idx;
        if (CompareKeys(binaryHeap[idx], binaryHeap[(idx - 1) / 2], pQueue->Keys)) {
            ShiftUp(pQueue, idx);
        }
        return true;
    }
    return false;
}

int Dequeue(TPQueue* pQueue) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    short* binaryHeap = pQueue->BinaryHeap;
    assert(len != 0);
    short minimum = binaryHeap[0];
    binaryHeap[0] = binaryHeap[len - 1];
    (pQueue->Len)--;
    ShiftDown(pQueue, 0);

    return minimum;
}

void DecreaseKey(TPQueue* pQueue, short value, int key) {
    short* order = pQueue->Order;
    int* keys = pQueue->Keys;
    int valueOrder = order[value];
    keys[value] = key;
    ShiftUp(pQueue, valueOrder);
}

bool IsEmpty(TPQueue* pQueue) {
    return (pQueue->Len == 0);
}
