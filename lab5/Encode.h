#ifndef LAB5_ENCODE_H
#define LAB5_ENCODE_H

#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>

typedef struct TBTree {
    struct TBTree* Left;
    struct TBTree* Right;
    int Symbol;
    long int Count;
} TBTree;

typedef struct TNstack {
    TBTree** stack;
    int maxLen;
    int len;
} TNStack;

typedef struct {
    FILE* File;
    unsigned char Symbol;
    unsigned int UsedSize;
} TFStream;

TBTree* CreateNode(TBTree* left, TBTree* right, int symbol, long int count);

TBTree* BuildHaffmanTree(const int* count, int alphabetLen);

void WriteLastInFile(TFStream* fWriter);

void SetCodes(TBTree* root, unsigned long* codes, unsigned* codeLens, unsigned long code, unsigned codeLen);

void PrintCompressedTree(TFStream* fWriter, TBTree* root);

void DeleteTree(TBTree* root);

void PrintEncodedLen(TFStream* fWriter, int const* count, int alphabetLen, unsigned const* codeLens);

void EncodeFile(FILE* in, TFStream* fWriter, unsigned long const* codes, unsigned const* codeLens);

#endif //LAB5_ENCODE_H
