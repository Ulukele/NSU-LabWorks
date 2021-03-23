#pragma once
#include <stdbool.h>

// Priority queue based on Binary heap

typedef struct {
    int Key;
    int Value;
} TPair;

typedef struct {
    TPair* BinaryHeap;
    int MaxLen;
    int Len;
} TPQueue;

TPQueue* CreateEmptyPQueue(int maxLen);

bool Enqueue(TPQueue* pQueue, TPair value);