#pragma once

typedef struct {
    short** Neighbours;
    short* NeighboursCount;
    int VerticesCount;
} TGraph;

typedef struct {
    short int Begin;
    short int End;
} TEdgeLight;
