#pragma once
#include "MemManager.h"

typedef struct TGraph {
    short** Neighbours;
    int** Weights;
    short* NeighboursCount;
    int VerticesCount;
} TGraph;

typedef struct {
    short int Begin;
    short int End;
} TEdgeLight;

TGraph* CreateEmptyGraph(int verticesCount, TMemNode* cleaner);

void AllocateNeighbours(TGraph* graph, TMemNode* cleaner);
