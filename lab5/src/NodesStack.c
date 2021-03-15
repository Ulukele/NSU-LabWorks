#include "NodesStack.h"

void RemoveFromStack(TNStack* stack, int index) {
    int stackLen = stack->len;
    TBTree** nodesStack = stack->stack;
    if (index < stackLen && index >= 0) {
        Swap(&nodesStack[index], &nodesStack[stackLen - 1]);
        stackLen--;
        stack->len = stackLen;
    }
}

void InsertInStack(TNStack* stack, TBTree* node) {
    int len = stack->len;
    if (len < stack->maxLen) {
        TBTree** nodesStack = stack->stack;
        nodesStack[len] = node;
        stack->len = len + 1;
    }
}

TBTree* PopMinIndex(TNStack* stack) {
    TBTree** nodesStack = stack->stack;
    int stackLen = stack->len;
    if (stackLen == 0) {
        return NULL;
    }
    int minIndex = 0;
    long int minCount = nodesStack[0]->Count;
    for (int i = 0; i < stackLen; ++i) {
        long int nodeCount = nodesStack[i]->Count;
        if (nodeCount < minCount) {
            minCount = nodeCount;
            minIndex = i;
        }
    }
    TBTree* result = nodesStack[minIndex];
    RemoveFromStack(stack, minIndex);
    return result;
}
