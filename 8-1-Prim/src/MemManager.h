#pragma once
#include <stddef.h>

typedef struct TMemNode TMemNode;

void* MallocAuto(TMemNode* trash, size_t size);

TMemNode* InitCleaner();

void CleanUp(TMemNode* trash);
