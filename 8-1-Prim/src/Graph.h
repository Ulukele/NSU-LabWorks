#pragma once
#include "MemClean.h"

typedef struct {
    short** Neighbours;
    int** Weights;
    short* NeighboursCount;
    int VerticesCount;
    TMemNode* Cleaner;
} TGraph;

typedef struct {
    short int Begin;
    short int End;
} TEdgeLight;

void PrintGraph(TGraph* graph);

void DeleteGraph(TGraph* graph);

TGraph* CreateEmptyGraph(int verticesCount);

void AllocateNeighbours(TGraph* graph);
