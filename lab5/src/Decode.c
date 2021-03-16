#include "Decode.h"

static TBTree* BuildTreeByFileRec(TFStream* fReader) {
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

void DecodeFile(TFStream* fReader, FILE* out, const TBTree* codesTree, unsigned int len, unsigned int lastLen) {
    unsigned int readBits = 0;
    unsigned int dataBits = len * 8u + lastLen;
    const TBTree* node = codesTree;
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
