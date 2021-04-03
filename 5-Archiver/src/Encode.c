#include "Encode.h"
#include <malloc.h>

TBTree* BuildHuffmanTree(const int* count, int alphabetLen) {
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

void SetCodes(const TBTree* root, unsigned long* codes, unsigned* codeLens, unsigned long code, unsigned codeLen) {
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

static void PrintCompressedTreeRec(const TBTree* root, TFStream* fWriter) {
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

void PrintCompressedTree(const TBTree* root, TFStream* fWriter) {
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

void PrintEncodedLen(TFStream* fWriter, const int* count, const unsigned* codeLens, int alphabetLen) {
    unsigned int compressedLen = 0;
    for (int i = 0; i < alphabetLen; ++i) {
        compressedLen += count[i] * codeLens[i];
    }
    for (int i = 0; i < 4; ++i) {
        WriteSmallInFile(fWriter, (unsigned char)(compressedLen % 256), 8u);
        compressedLen /= 256;
    }
}

void EncodeFile(FILE* in, TFStream* fWriter, const unsigned long* codes, const unsigned* codeLens) {
    int symbol = 0;
    while (symbol != EOF) {
        symbol = getc(in);
        if (symbol == EOF) {
            break;
        }
        WriteBigInFile(fWriter, codes[symbol], codeLens[symbol]);
    }
}
