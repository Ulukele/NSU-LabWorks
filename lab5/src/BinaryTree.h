#ifndef LAB5_BINARYTREE_H
#define LAB5_BINARYTREE_H

#include <malloc.h>

typedef struct TBTree {
    struct TBTree* Left;
    struct TBTree* Right;
    int Symbol;
    long int Count;
} TBTree;

TBTree* CreateNode(TBTree* left, TBTree* right, int symbol, long int count);

void Swap(TBTree** first, TBTree** second);

void DeleteTree(TBTree* root);

#endif //LAB5_BINARYTREE_H
