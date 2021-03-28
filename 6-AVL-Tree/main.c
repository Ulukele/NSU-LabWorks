#include <stdio.h>
#include <malloc.h>
#include <assert.h>

int Max(int first, int second) {
    if (first > second) {
        return first;
    }
    return second;
}

// AVL tree
struct TreeNode {
    int Value;
    int Height;
    struct TreeNode* Left;
    struct TreeNode* Right;
};
typedef struct TreeNode TTree;

TTree* CreateNode(int value, int height, TTree* memory, int* last) {
    TTree* node = &memory[*last];
    (*last)++;
    assert(node != NULL);

    *node = (TTree){value, height, NULL, NULL};
    return node;
}

// Safe way to check height
int GetHeight(TTree* node) {
    if (node == NULL) {
        return 0;
    }
    return node->Height;
}

// Rebuild and return root of tree after Left rotation
TTree* LeftRotate(TTree* node) {
    TTree* new_top = node->Right;
    node->Right = new_top->Left;
    new_top->Left = node;
    // Correct height
    node->Height = Max(GetHeight(node->Left), GetHeight(node->Right)) + 1;
    new_top->Height = Max(GetHeight(new_top->Left), GetHeight(new_top->Right)) + 1;
    return new_top;
}

// Rebuild and return root of tree after Right rotation
TTree* RightRotate(TTree* node) {
    TTree* new_top = node->Left;
    node->Left = new_top->Right;
    new_top->Right = node;
    // Correct height
    node->Height = Max(GetHeight(node->Left), GetHeight(node->Right)) + 1;
    new_top->Height = Max(GetHeight(new_top->Left), GetHeight(new_top->Right)) + 1;
    return new_top;
}

// Rebuild and return root of tree after Big Left rotation
TTree* BigLeftRotate(TTree* node) {
    node->Right = RightRotate(node->Right);
    return LeftRotate(node);
}

// Rebuild and return root of tree after Big Right rotation
TTree* BigRightRotate(TTree* node) {
    node->Left = LeftRotate(node->Left);
    return RightRotate(node);
}

// Rebuild and return root of tree with 1 more (inserted) node
TTree* Insert(TTree* root, int value, TTree* memory, int* last) {
    if (root == NULL) {
        return CreateNode(value, 1, memory, last);
    }
    if (value >= root->Value) {
        root->Right = Insert(root->Right, value, memory, last);
    }
    else {
        root->Left = Insert(root->Left, value, memory, last);
    }

    TTree* left = root->Left;
    TTree * right = root->Right;
    int left_h = GetHeight(left);
    int right_h = GetHeight(right);

    root->Height = Max(left_h, right_h) + 1;

    if (left_h - right_h == 2) {
        int left_sub_h = GetHeight(left->Left);
        int right_sub_h = GetHeight(left->Right);
        if (right_sub_h <= left_sub_h) {
            root = RightRotate(root);
        }
        else {
            root = BigRightRotate(root);
        }
    }
    if (right_h - left_h == 2) {
        int left_sub_h = GetHeight(right->Left);
        int right_sub_h = GetHeight(right->Right);
        if (left_sub_h <= right_sub_h) {
            root = LeftRotate(root);
        }
        else {
            root = BigLeftRotate(root);
        }
    }
    return root;
}

int main() {

    int n;
    if (scanf("%d", &n) != 1) {
        return 0;
    }

    TTree* tree = NULL;
    TTree* memory = malloc(sizeof(*memory) * n);
    int last = 0;

    for (int i = 0; i < n; ++i) {
        int node_value;
        if (scanf("%d", &node_value) != 1) {
            free(memory);
            return 0;
        }
        tree = Insert(tree, node_value, memory, &last);
    }

    printf("%d", GetHeight(tree));
    free(memory);

    return 0;
}
