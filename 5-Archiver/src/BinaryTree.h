#pragma once

typedef struct TBTree {
    struct TBTree* Left;
    struct TBTree* Right;
    int Symbol;
    long int Count;
} TBTree;

TBTree* CreateNode(const TBTree* left, const TBTree* right, int symbol, long int count);

void Swap(TBTree** first, TBTree** second);

void DeleteTree(TBTree* root);
