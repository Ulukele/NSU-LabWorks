#pragma once
#include <stdbool.h>

// Priority queue based on Binary heap

typedef struct {
    short* BinaryHeap;
    int* Keys;
    short* Order;
    int MaxLen;
    int Len;
} TPQueue;

TPQueue* CreateEmptyPQueue(int maxLen, int* keys, short* order);

void DeletePQueue(TPQueue* pQueue);

bool Enqueue(TPQueue* pQueue, short value);

int Dequeue(TPQueue* pQueue);

void DecreaseKey(TPQueue* pQueue, short value, int key);

bool IsEmpty(TPQueue* pQueue);
