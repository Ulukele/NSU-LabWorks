#include "PriorityQueue.h"
#include <malloc.h>
#include <stdbool.h>

static void Swap(short* first, short* second) {
    short holder = *first;
    *first = *second;
    *second = holder;
}

static bool CompareKeys(const short* first, const short* second, const int* keys) {
    return (keys[*first] == PQUEUE_INF_KEY || keys[*first] > keys[*second]);
}

TPQueue* CreateEmptyPQueue(int maxLen, int* keys) {
    short* binaryHeap = malloc(sizeof(*binaryHeap) * maxLen);
    TPQueue* pQueue = malloc(sizeof(*pQueue));
    if (binaryHeap == NULL || pQueue == NULL) {
        free(binaryHeap);
        free(pQueue);
        return NULL;
    }
    pQueue->BinaryHeap = binaryHeap;
    pQueue->Keys = keys;
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
        if (right < len && CompareKeys(&binaryHeap[left], &binaryHeap[right], pQueue->keys)) {
            minimum = right;
        }
        if (!CompareKeys(&binaryHeap[idx], &binaryHeap[minimum], pQueue->keys)) {
            break;
        }
        Swap(&binaryHeap[idx], &binaryHeap[minimum]);
        idx = minimum;
    }
}

static void ShiftUp(TPQueue* pQueue, int idx) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    short* binaryHeap = pQueue->BinaryHeap;

    int parent = (idx - 1) / 2;
    while (CompareKeys(&binaryHeap[parent], &binaryHeap[idx], pQueue->keys)) {
        Swap(&binaryHeap[idx], &binaryHeap[parent]);
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

bool Enqueue(TPQueue* pQueue, short value) {
    int len = pQueue->Len;
    int maxLen = pQueue->MaxLen;
    short* binaryHeap = pQueue->BinaryHeap;

    if (len < maxLen) {
        int idx = len;
        len++;
        binaryHeap[idx] = value;
        if (CompareKeys(&binaryHeap[idx], &binaryHeap[(idx - 1) / 2], pQueue->keys)) {
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
    short minimum = binaryHeap[0];
    binaryHeap[0] = binaryHeap[len - 1];
    (pQueue->Len)--;
    ShiftDown(pQueue, 0);

    return minimum;
}

bool IsEmpty(TPQueue* pQueue) {
    return (pQueue->Len == 0);
}
