#include "PriorityQueue.h"
#include <malloc.h>

static void Swap(TPair* first, TPair* second) {
    TPair holder = *first;
    *first = *second;
    *second = holder;
}

TPQueue* CreateEmptyPQueue(int maxLen) {
    TPair* binaryHeap = malloc(sizeof(*binaryHeap) * maxLen);
    TPQueue* pQueue = malloc(sizeof(*pQueue));
    if (binaryHeap == NULL || pQueue == NULL) {
        free(binaryHeap);
        free(pQueue);
        return NULL;
    }
    pQueue->BinaryHeap = binaryHeap;
    pQueue->MaxLen = maxLen;
    pQueue->Len = 0;
    return pQueue;
}

static void ShiftUp(TPQueue* pQueue, int idx) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    TPair* binaryHeap = pQueue->BinaryHeap;

    int parent = (idx - 1) / 2;
    while (binaryHeap[idx].Key < binaryHeap[parent].Key) {
        Swap(&binaryHeap[idx], &binaryHeap[parent]);
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

bool Enqueue(TPQueue* pQueue, TPair value) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    TPair* binaryHeap = pQueue->BinaryHeap;

    if (len < maxLen) {
        int idx = len;
        len++;
        binaryHeap[idx] = value;
        if (binaryHeap[idx].Key > binaryHeap[(idx - 1) / 2].Key) {
            ShiftUp(pQueue, idx);
        }
        return true;
    }
    return false;
}
