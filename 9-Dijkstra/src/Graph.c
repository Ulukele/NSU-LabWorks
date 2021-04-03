#include "Graph.h"
#include <malloc.h>

TGraph* CreateEmptyGraph(int verticesCount, TMemNode* cleaner) {
    if (cleaner == NULL) {
        return NULL;
    }
    TGraph* graph = MallocAuto(cleaner, sizeof(*graph));
    int* weightsMatrix = MallocAuto(cleaner, sizeof(*weightsMatrix) * verticesCount * verticesCount);
    if (graph == NULL || weightsMatrix == NULL) {
        free(graph);
        free(weightsMatrix);
        return NULL;
    }

    graph->VerticesCount = verticesCount;
    graph->WeightsMatrix = weightsMatrix;
    return graph;
}
