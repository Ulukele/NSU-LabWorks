#include "Decode.h"

void ReadFirstFromFile(TFStream* fReader) {
    fReader->Symbol = getc(fReader->File);
}

unsigned char ReadSmallFromFile(TFStream* fReader, unsigned int codeLen) {
    unsigned int readSize = fReader->UsedSize;
    unsigned int restSize = 8u - readSize;
    unsigned char code = 0;

    if (codeLen <= restSize) {
        code = (unsigned char)(fReader->Symbol << (restSize - codeLen)) >> (8u - codeLen);
        readSize += codeLen;
        assert(readSize <= 8u);
        if (readSize == 8u) {
            readSize = 0;
            ReadFirstFromFile(fReader);
        }
        fReader->UsedSize = readSize;
    }
    else {
        code += ReadSmallFromFile(fReader, restSize);
        code += ReadSmallFromFile(fReader, codeLen - restSize) << restSize;
    }
    return code;
}

TBTree* BuildTreeByFileRec(TFStream* fReader) {
    unsigned char isTree = ReadSmallFromFile(fReader, 1);
    TBTree* root = NULL;
    if (isTree) {
        TBTree* left = BuildTreeByFileRec(fReader);
        TBTree* right = BuildTreeByFileRec(fReader);
        root = CreateNode(left, right, -1, 0);
    }
    else {
        unsigned char symbol = ReadSmallFromFile(fReader, 8);
        root = CreateNode(NULL, NULL, symbol, 0);
    }

    return root;
}

TBTree* BuildTreeByFile(TFStream* fReader) {
    TBTree* root = NULL;
    if (fReader->Symbol % 2 == 0) {
        TBTree* node = BuildTreeByFileRec(fReader);
        root = CreateNode(node, NULL, -1, 0);
    }
    else {
        root = BuildTreeByFileRec(fReader);
    }
    return root;
}

void DecodeFile(TFStream* fReader, FILE* out, TBTree* codesTree, unsigned int len, unsigned int lastLen) {
    unsigned int readBits = 0;
    unsigned int dataBits = len * 8u + lastLen;
    TBTree* node = codesTree;
    while (readBits < dataBits) {
        unsigned char code = ReadSmallFromFile(fReader, 1);
        readBits++;
        if (code) {
            node = node->Right;
        }
        else {
            node = node->Left;
        }
        if (node->Symbol != EOF) {
            putc(node->Symbol, out);
            node = codesTree;
        }
    }
}
