#ifndef LAB5_DECODE_H
#define LAB5_DECODE_H

#include "FileStream.h"
#include "BinaryTree.h"

TBTree* BuildTreeByFile(TFStream* fReader);

void DecodeFile(TFStream* fReader, FILE* out, TBTree* codesTree, unsigned int len, unsigned int lastLen);

#endif //LAB5_DECODE_H
