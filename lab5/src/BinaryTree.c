#include "BinaryTree.h"
#include <malloc.h>

TBTree* CreateNode(const TBTree* left, const TBTree* right, int symbol, long int count) {
    TBTree* node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->Left = (TBTree*)left;
    node->Right = (TBTree*)right;
    node->Symbol = symbol;
    node->Count = count;
    return node;
}

void Swap(TBTree** first, TBTree** second) {
    TBTree* holder = *first;
    *first = *second;
    *second = holder;
}

void DeleteTree(TBTree* root) {
    if (root != NULL) {
        DeleteTree(root->Left);
        DeleteTree(root->Right);
        free(root);
    }
}
