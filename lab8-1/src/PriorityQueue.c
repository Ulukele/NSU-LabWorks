#include "PriorityQueue.h"
#include <malloc.h>
#include <stdbool.h>

static void Swap(TPair* first, TPair* second) {
    TPair holder = *first;
    *first = *second;
    *second = holder;
}

static bool CompareKeys(const TPair* first, const TPair* second) {
    return (first->Key == PQUEUE_INF_KEY || first->Key > second->Key);
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

static void ShiftDown(TPQueue* pQueue, int idx) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    TPair* binaryHeap = pQueue->BinaryHeap;

    while (3 * idx + 1 < len) {
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        int minimum = left;
        if (right < len && CompareKeys(&binaryHeap[left], &binaryHeap[right])) {
            minimum = right;
        }
        if (!CompareKeys(&binaryHeap[idx], &binaryHeap[minimum])) {
            break;
        }
        Swap(&binaryHeap[idx], &binaryHeap[minimum]);
        idx = minimum;
    }
}

static void ShiftUp(TPQueue* pQueue, int idx) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    TPair* binaryHeap = pQueue->BinaryHeap;

    int parent = (idx - 1) / 2;
    while (CompareKeys(&binaryHeap[parent], &binaryHeap[idx])) {
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
        if (CompareKeys(&binaryHeap[idx], &binaryHeap[(idx - 1) / 2])) {
            ShiftUp(pQueue, idx);
        }
        return true;
    }
    return false;
}

int Dequeue(TPQueue* pQueue) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    TPair* binaryHeap = pQueue->BinaryHeap;
    int minimum = binaryHeap[0].Value;
    binaryHeap[0] = binaryHeap[len - 1];
    (pQueue->Len)--;
    ShiftDown(pQueue, 0);

    return minimum;
}

bool IsEmpty(TPQueue* pQueue) {
    return (pQueue->Len == 0);
}
