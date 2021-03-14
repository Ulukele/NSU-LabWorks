#include "Encode.h"

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

TBTree* BuildHaffmanTree(const int* count, int alphabetLen) {
    int different = 0;
    for (int i = 0; i < alphabetLen; ++i) {
        if (count[i] != 0) {
            different++;
        }
    }
    TNStack nodesStack;
    nodesStack.maxLen = different + 1;
    nodesStack.len = 0;
    TBTree** nodes = malloc(sizeof(TBTree*) * nodesStack.maxLen);
    if (nodes == NULL) {
        return NULL;
    }
    nodesStack.stack = nodes;
    for (int i = 0; i < alphabetLen; ++i) {
        if (count[i] != 0) {
            TBTree* node = CreateNode(NULL, NULL, i, count[i]);
            InsertInStack(&nodesStack, node);
        }
    }

    do {
        TBTree* firstNode = PopMinIndex(&nodesStack);
        TBTree* secondNode = PopMinIndex(&nodesStack);
        long int newCount = 0;
        if (firstNode) {
            newCount += firstNode->Count;
        }
        if (secondNode) {
            newCount += secondNode->Count;
        }
        TBTree* newNode = CreateNode(firstNode, secondNode, -1, newCount);
        InsertInStack(&nodesStack, newNode);
    } while (nodesStack.len != 1);

    TBTree* root = nodes[0];
    free(nodes);
    return root;
}

void DeleteTree(TBTree* root) {
    if (root != NULL) {
        DeleteTree(root->Left);
        DeleteTree(root->Right);
        free(root);
    }
}

void WriteLastInFile(TFStream* fWriter) {
    if (fWriter->UsedSize != 0) {
        putc(fWriter->Symbol, fWriter->File);
        fWriter->UsedSize = 0;
    }
}

void WriteSmallInFile(TFStream* fWriter, unsigned char code, unsigned int codeLen) {
    unsigned int writeSize = fWriter->UsedSize;
    unsigned int restSize = 8u - writeSize;

    if (codeLen <= restSize) {
        fWriter->Symbol += code << writeSize;
        writeSize += codeLen;
        assert(writeSize <= 8u);
        if (writeSize == 8u) {
            writeSize = 0;
            putc(fWriter->Symbol, fWriter->File);
            fWriter->Symbol = 0;
        }
        fWriter->UsedSize = writeSize;
    }
    else {
        unsigned char second = code >> restSize;
        unsigned char first = code - (second << restSize);
        WriteSmallInFile(fWriter, first, restSize);
        WriteSmallInFile(fWriter, second, codeLen - restSize);
    }
}

void WriteBigInFile(TFStream* fWriter, unsigned long int code, unsigned int codeLen) {
    unsigned long int mask = 255;
    unsigned int len = codeLen / 8u;
    for (unsigned int i = 0; i < len; ++i) {
        unsigned char symbol = (code & mask) >> (8u * i);
        WriteSmallInFile(fWriter, symbol, 8u);
        mask = mask << 8u;
    }
    if (codeLen % 8u != 0) {
        unsigned char symbol = (code & mask) >> (8u * len);
        WriteSmallInFile(fWriter, symbol, codeLen % 8u);
    }
}

void SetCodes(TBTree* root, unsigned long int* codes, unsigned int* codeLens, unsigned long code, unsigned codeLen) {
    if (root == NULL) {
        return;
    }
    int symbol = root->Symbol;
    if (symbol != -1) {
        codes[symbol] = code;
        codeLens[symbol] = codeLen;
    }
    else {
        SetCodes(root->Left, codes, codeLens, code, codeLen + 1u);
        SetCodes(root->Right, codes, codeLens, code + (1Lu << codeLen), codeLen + 1u);
    }
}

void PrintCompressedTreeRec(TBTree* root, TFStream* fWriter) {
    if (!root) {
        WriteSmallInFile(fWriter, 0, 1u);
        WriteSmallInFile(fWriter, 0, 8u);
        return;
    }
    if (root->Symbol != -1) {
        WriteSmallInFile(fWriter, 0, 1u);
        WriteSmallInFile(fWriter, (unsigned char)root->Symbol, 8u);
    }
    else {
        WriteSmallInFile(fWriter, 1u, 1u);
        PrintCompressedTreeRec(root->Left, fWriter);
        PrintCompressedTreeRec(root->Right, fWriter);
    }
}

void PrintCompressedTree(TFStream* fWriter, TBTree* root) {
    if (!root->Left) {
        WriteSmallInFile(fWriter, 0, 1u);
        WriteSmallInFile(fWriter, 0, 8u);
    }
    else if (!root->Right) {
        TBTree* node = root->Left;
        WriteSmallInFile(fWriter, 0, 1u);
        WriteSmallInFile(fWriter, (unsigned char)node->Symbol, 8u);
    }
    else {
        PrintCompressedTreeRec(root, fWriter);
    }
}

void PrintEncodedLen(TFStream* fWriter, int const* count, int alphabetLen, unsigned const* codeLens) {
    unsigned int compressedLen = 0;
    for (int i = 0; i < alphabetLen; ++i) {
        compressedLen += count[i] * codeLens[i];
    }
    for (int i = 0; i < 4; ++i) {
        WriteSmallInFile(fWriter, (unsigned char)(compressedLen % 256), 8u);
        compressedLen /= 256;
    }
}

void EncodeFile(FILE* in, TFStream* fWriter, unsigned long const* codes, unsigned const* codeLens) {
    int symbol = 0;
    while (symbol != EOF) {
        symbol = getc(in);
        if (symbol == EOF) {
            break;
        }
        WriteBigInFile(fWriter, codes[symbol], codeLens[symbol]);
    }
}
