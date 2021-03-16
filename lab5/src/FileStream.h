#pragma once
#include <stdio.h>

typedef struct {
    FILE* File;
    unsigned char Symbol;
    unsigned int UsedSize;
} TFStream;

void WriteLastInFile(TFStream* fWriter);

void WriteSmallInFile(TFStream* fWriter, unsigned char code, unsigned int codeLen);

void WriteBigInFile(TFStream* fWriter, unsigned long int code, unsigned int codeLen);

void ReadFirstFromFile(TFStream* fReader);

unsigned char ReadSmallFromFile(TFStream* fReader, unsigned int codeLen);
