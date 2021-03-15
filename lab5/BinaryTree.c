#include "BinaryTree.h"

TBTree* CreateNode(TBTree* left, TBTree* right, int symbol, long int count) {
    TBTree* node = malloc(sizeof(*node));
    if (node == NULL) {
        return NULL;
    }
    node->Left = left;
    node->Right = right;
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
