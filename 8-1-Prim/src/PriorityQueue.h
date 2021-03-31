#pragma once
#include <stdbool.h>
#define PQUEUE_INF_KEY -1

// Priority queue based on Binary heap

typedef struct {
    short* BinaryHeap;
    int* Keys;
    int MaxLen;
    int Len;
} TPQueue;

TPQueue* CreateEmptyPQueue(int maxLen, int* keys);

void DeletePQueue(TPQueue* pQueue);

bool Enqueue(TPQueue* pQueue, short value);

int Dequeue(TPQueue* pQueue);

bool IsEmpty(TPQueue* pQueue);
