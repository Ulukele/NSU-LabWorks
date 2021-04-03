#include "MemManager.h"
#include <malloc.h>

struct TMemNode {
    void* Ptr;
    struct TMemNode* Next;
};

static TMemNode* CreateMemNode(void* ptr, TMemNode* next) {
    TMemNode* memNode = malloc(sizeof(*memNode));
    if (memNode == NULL) {
        return NULL;
    }
    memNode->Ptr = ptr;
    memNode->Next = next;
    return memNode;
}

static void DeleteNode(TMemNode* node) {
    if (node != NULL) {
        free(node->Ptr);
        free(node);
    }
}

void* MallocAuto(TMemNode* trash, size_t size) {
    if (trash == NULL) {
        return NULL;
    }
    void* ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;
    }
    void* new = CreateMemNode(ptr, trash->Next);
    trash->Next = new;

    return ptr;
}

TMemNode* InitCleaner() {
    return CreateMemNode(NULL, NULL);
}

void CleanUp(TMemNode* trash) {
    while (trash != NULL) {
        void* next = trash->Next;
        DeleteNode(trash);
        trash = next;
    }
}
