#include "Graph.h"
#include <malloc.h>

void PrintGraph(TGraph* graph) {
    short** neighbours = graph->Neighbours;
    int** weights = graph->Weights;
    short* neighboursCount = graph->NeighboursCount;
    int verticesCount = graph->VerticesCount;
    printf("Graph:\n");
    printf("Vertices Count: %d\n", verticesCount);
    for (int i = 0; i < verticesCount; ++i) {
        printf("%d: ", i);
        for (int j = 0; j < neighboursCount[i]; ++j) {
            printf("(%hd %d) ", neighbours[i][j], weights[i][j]);
        }
        printf("\n");
    }
}

void DeleteGraph(TGraph* graph) {
    if (graph == NULL) {
        return;
    }
    int verticesCount = graph->VerticesCount;
    free(graph->NeighboursCount);
    if (graph->Neighbours != NULL) {
        for (int i = 0; i < verticesCount; ++i) {
            free(graph->Neighbours[i]);
        }
        free(graph->Neighbours);
    }
    if (graph->Weights != NULL) {
        for (int i = 0; i < verticesCount; ++i) {
            free(graph->Weights[i]);
        }
        free(graph->Weights);
    }
    free(graph);
    
}

TGraph* CreateEmptyGraph(int verticesCount) {
    TGraph* graph = malloc(sizeof(*graph));
    if (graph == NULL) {
        return NULL;
    }
    short** neighbours = malloc(sizeof(short*) * verticesCount);
    int** weights = malloc(sizeof(int*) * verticesCount);
    short* neighboursCount = calloc(verticesCount, sizeof(*neighboursCount));
    graph->Neighbours = neighbours;
    graph->Weights = weights;
    graph->NeighboursCount = neighboursCount;
    graph->VerticesCount = verticesCount;
    if (neighbours == NULL || weights == NULL || neighboursCount == NULL) {
        DeleteGraph(graph);
    }
    return graph;
}
