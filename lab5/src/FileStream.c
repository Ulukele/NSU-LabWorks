#include "FileStream.h"

void WriteLastInFile(TFStream* fWriter) {
    if (fWriter->UsedSize != 0) {
        putc(fWriter->Symbol, fWriter->File);
        fWriter->UsedSize = 0;
    }
}

void WriteSmallInFile(TFStream* fWriter, unsigned char code, unsigned int codeLen) {
    unsigned int writeSize = fWriter->UsedSize;
    unsigned int restSize = 8u - writeSize;

    if (codeLen <= restSize) {
        fWriter->Symbol += code << writeSize;
        writeSize += codeLen;
        assert(writeSize <= 8u);
        if (writeSize == 8u) {
            writeSize = 0;
            putc(fWriter->Symbol, fWriter->File);
            fWriter->Symbol = 0;
        }
        fWriter->UsedSize = writeSize;
    }
    else {
        unsigned char second = code >> restSize;
        unsigned char first = code - (second << restSize);
        WriteSmallInFile(fWriter, first, restSize);
        WriteSmallInFile(fWriter, second, codeLen - restSize);
    }
}

void WriteBigInFile(TFStream* fWriter, unsigned long int code, unsigned int codeLen) {
    unsigned long int mask = 255;
    unsigned int len = codeLen / 8u;
    for (unsigned int i = 0; i < len; ++i) {
        unsigned char symbol = (code & mask) >> (8u * i);
        WriteSmallInFile(fWriter, symbol, 8u);
        mask = mask << 8u;
    }
    if (codeLen % 8u != 0) {
        unsigned char symbol = (code & mask) >> (8u * len);
        WriteSmallInFile(fWriter, symbol, codeLen % 8u);
    }
}

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
