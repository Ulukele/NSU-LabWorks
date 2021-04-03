#include "Graph.h"
#include <malloc.h>

TGraph* CreateEmptyGraph(int verticesCount, TMemNode* cleaner) {
    TGraph* graph = MallocAuto(cleaner, sizeof(*graph));
    if (graph == NULL || cleaner == NULL) {
        free(graph);
        free(cleaner);
        return NULL;
    }
    short** neighbours = MallocAuto(cleaner, sizeof(short*) * verticesCount);
    int** weights = MallocAuto(cleaner, sizeof(int*) * verticesCount);
    short* neighboursCount = MallocAuto(cleaner, sizeof(*neighboursCount) * verticesCount);
    for (int i = 0; i < verticesCount; ++i) {
        neighboursCount[i] = 0;
    }
    graph->Neighbours = neighbours;
    graph->Weights = weights;
    graph->NeighboursCount = neighboursCount;
    graph->VerticesCount = verticesCount;
    if (neighbours == NULL || weights == NULL || neighboursCount == NULL) {
        CleanUp(cleaner);
    }
    return graph;
}

void AllocateNeighbours(TGraph* graph, TMemNode* cleaner) {
    short* neighboursCount = graph->NeighboursCount;
    short** neighbours = graph->Neighbours; 
    int** weights = graph->Weights;
    int verticesCount = graph->VerticesCount;

    for (int i = 0; i < verticesCount; ++i) {
        neighbours[i] = MallocAuto(cleaner, sizeof(short) * neighboursCount[i]);
        weights[i] = MallocAuto(cleaner, sizeof(int) * neighboursCount[i]);
    }
}
