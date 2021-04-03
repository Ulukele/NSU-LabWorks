#pragma once

#include "FileStream.h"
#include "BinaryTree.h"

TBTree* BuildTreeByFile(TFStream* fReader);

void DecodeFile(TFStream* fReader, FILE* out, const TBTree* codesTree, unsigned int bitsCount);
