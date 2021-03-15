#ifndef LAB5_NODESSTACK_H
#define LAB5_NODESSTACK_H

#include "BinaryTree.h"

typedef struct TNstack {
    TBTree** stack;
    int maxLen;
    int len;
} TNStack;

void RemoveFromStack(TNStack* stack, int index);

void InsertInStack(TNStack* stack, TBTree* node);

TBTree* PopMinIndex(TNStack* stack);

#endif //LAB5_NODESSTACK_H
