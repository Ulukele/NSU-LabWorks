#include <stdio.h>
#include "Encode.h"
#include "Decode.h"

#define INPUT "in.txt"
#define OUTPUT "out.txt"

#define ALPHABET_LEN 256

void SkipChars(FILE* file, unsigned int count) {
    for (unsigned int i = 0; i < count; ++i) {
        getc(file);
    }
}

int main() {
    FILE* in = fopen(INPUT, "rb");
    FILE* out = fopen(OUTPUT, "wb");
    char c;
    if (fscanf(in,"%c", &c) != 1) {
        fclose(in);
        fclose(out);
        return 0;
    }
    SkipChars(in, 2);
    if (c == 'c') {
        int count[ALPHABET_LEN] = {0};

        int symbol = 0;
        while (symbol != EOF) {
            symbol = getc(in);
            if (symbol != EOF) {
                count[symbol]++;
            }
        }

        TBTree* root = BuildHaffmanTree(count, ALPHABET_LEN);

        unsigned long int codes[ALPHABET_LEN] = {0};
        unsigned int codeLens[ALPHABET_LEN] = {0};
        SetCodes(root, codes, codeLens, 0, 0);

        fclose(in);
        in = fopen(INPUT, "rb");
        SkipChars(in, 3);

        TFStream fWriter = {out, 0, 0};
        PrintCompressedTree(&fWriter, root);
        PrintEncodedLen(&fWriter, count, ALPHABET_LEN, codeLens);
        EncodeFile(in, &fWriter, codes, codeLens);
        WriteLastInFile(&fWriter);

        DeleteTree(root);
    }
    if (c == 'd') {
        TFStream fReader = {in, 0, 0};
        ReadFirstFromFile(&fReader);

        TBTree* codesTree = BuildTreeByFile(&fReader);

        unsigned int dataBits = 0;
        unsigned int deg = 1;
        for (int i = 0; i < 4; ++i) {
            dataBits += ReadSmallFromFile(&fReader, 8u) * deg;
            deg *= 256;
        }
        unsigned int dataLen = dataBits / 8u;
        unsigned int dataLastLen = dataBits % 8u;

        DecodeFile(&fReader, out, codesTree, dataLen, dataLastLen);
        DeleteTree(codesTree);
    }
    fclose(in);
    fclose(out);
    return 0;
}
