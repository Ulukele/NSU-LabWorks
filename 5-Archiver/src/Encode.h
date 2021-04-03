#pragma once
#include <stdio.h>
#include "FileStream.h"
#include "BinaryTree.h"
#include "NodesStack.h"

TBTree* BuildHuffmanTree(const int* count, int alphabetLen);

void SetCodes(const TBTree* root, unsigned long* codes, unsigned* codeLens, unsigned long code, unsigned codeLen);

void PrintCompressedTree(const TBTree* root, TFStream* fWriter);

void PrintEncodedLen(TFStream* fWriter, const int* count, const unsigned* codeLens, int alphabetLen);

void EncodeFile(FILE* in, TFStream* fWriter, const unsigned long* codes, const unsigned* codeLens);
