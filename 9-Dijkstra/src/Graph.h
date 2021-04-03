#pragma once
#include "MemManager.h"

typedef struct TGraph {
    int* WeightsMatrix;
    int VerticesCount;
} TGraph;

TGraph* CreateEmptyGraph(int verticesCount, TMemNode* cleaner);
