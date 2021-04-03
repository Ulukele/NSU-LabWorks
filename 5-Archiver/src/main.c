#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Encode.h"
#include "Decode.h"

#define INPUT "in.txt"
#define OUTPUT "out.txt"

#define ALPHABET_LEN 256

bool ParseParameters(int argc, char** argv, char* mode, FILE** inputFile, FILE** outputFile) {
    bool parsed = false;
    if (argc == 1) {
        *inputFile = fopen(INPUT, "rb");
        *outputFile = fopen(OUTPUT, "wb");
        if (fscanf(*inputFile,"%c", mode) != 1) {
            fclose(*inputFile);
            fclose(*outputFile);
            printf("No mode specified in %s.\n", INPUT);
            exit(0);
        }
        fseek(*inputFile, 2, SEEK_CUR);
    }
    else if (argc == 2) {
        printf("Specified only mode.\n");
        *inputFile = fopen(INPUT, "rb");
        *outputFile = fopen(OUTPUT, "wb");
        *mode = argv[1][0];
    }
    else if (argc == 3) {
        printf("Specified only one file.\nChoose 2 files\n");
        exit(0);
    }
    else if (argc == 4) {
        *mode = argv[1][0];
        *inputFile = fopen(argv[2], "rb");
        *outputFile = fopen(argv[3], "wb");
        parsed = true;
    }
    else {
        printf("Too much parameters!\n");
        exit(0);
    }
    return parsed;
}

int main(int argc, char** argv) {
    FILE* in;
    FILE* out;
    char mode;

    bool parsed = ParseParameters(argc, argv, &mode, &in, &out);
    
    if (mode == 'c') {
        int count[ALPHABET_LEN] = {0};

        int symbol = 0;
        while (symbol != EOF) {
            symbol = getc(in);
            if (symbol != EOF) {
                count[symbol]++;
            }
        }

        TBTree* root = BuildHuffmanTree(count, ALPHABET_LEN);

        unsigned long int codes[ALPHABET_LEN] = {0};
        unsigned int codeLens[ALPHABET_LEN] = {0};
        SetCodes(root, codes, codeLens, 0, 0);

        if (parsed) {
            fseek(in, 0, 0);
        }
        else {
            fseek(in, 3, 0);
        }

        TFStream fWriter = {out, 0, 0};
        PrintCompressedTree(root, &fWriter);
        PrintEncodedLen(&fWriter, count, codeLens, ALPHABET_LEN);
        EncodeFile(in, &fWriter, codes, codeLens);
        WriteLastInFile(&fWriter);

        DeleteTree(root);
    }
    if (mode == 'd') {
        TFStream fReader = {in, 0, 0};
        ReadFirstFromFile(&fReader);

        TBTree* codesTree = BuildTreeByFile(&fReader);

        unsigned int dataBits = 0;
        unsigned int deg = 1;
        for (int i = 0; i < 4; ++i) {
            dataBits += ReadSmallFromFile(&fReader, 8u) * deg;
            deg *= 256;
        }

        DecodeFile(&fReader, out, codesTree, dataBits);
        DeleteTree(codesTree);
    }
    fclose(in);
    fclose(out);
    return 0;
}
