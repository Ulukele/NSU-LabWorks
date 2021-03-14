#ifndef LAB5_DECODE_H
#define LAB5_DECODE_H

#include "Encode.h"

void ReadFirstFromFile(TFStream* fReader);

unsigned char ReadSmallFromFile(TFStream* fReader, unsigned int codeLen);

TBTree* BuildTreeByFile(TFStream* fReader);

void DecodeFile(TFStream* fReader, FILE* out, TBTree* codesTree, unsigned int len, unsigned int lastLen);

#endif //LAB5_DECODE_H
