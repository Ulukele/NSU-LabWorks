#ifndef LAB5_ENCODE_H
#define LAB5_ENCODE_H

#include <stdio.h>
#include <malloc.h>
#include "FileStream.h"
#include "BinaryTree.h"
#include "NodesStack.h"

TBTree* CreateNode(TBTree* left, TBTree* right, int symbol, long int count);

TBTree* BuildHaffmanTree(const int* count, int alphabetLen);

void SetCodes(TBTree* root, unsigned long* codes, unsigned* codeLens, unsigned long code, unsigned codeLen);

void PrintCompressedTree(TFStream* fWriter, TBTree* root);

void DeleteTree(TBTree* root);

void PrintEncodedLen(TFStream* fWriter, int const* count, int alphabetLen, unsigned const* codeLens);

void EncodeFile(FILE* in, TFStream* fWriter, unsigned long const* codes, unsigned const* codeLens);

#endif //LAB5_ENCODE_H
