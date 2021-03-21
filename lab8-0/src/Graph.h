#pragma once

typedef struct {
    short int Begin;
    short int End;
    int Weight;
} TEdge;

typedef struct {
    short int Begin;
    short int End;
} TEdgeLight;

typedef struct {
    TEdge* Edges;
    int VerticesCount;
    int EdgesCount;
} TGraph;

void SortEdges(TEdge* edges, int edgesCount);
