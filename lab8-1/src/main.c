#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include "Graph.h"
#include "PriorityQueue.h"

void SkipNum(FILE* file, int count) {
    int skip;
    for (int i = 0; i < count; ++i) {
        fscanf(file, "%d", &skip);
    }
}

TEdgeLight* FindSpanningTree(TGraph* graph) {
    int verticesCount = graph->VerticesCount;
    short** neighbours = graph->Neighbours;
    short* neighboursCount = graph->NeighboursCount;

    if (verticesCount == 0) {
        return NULL;
    }
    TEdgeLight* spanningTree = malloc((verticesCount - 1) * sizeof(*spanningTree));
    if (spanningTree == NULL) {
        return NULL;
    }
    short* parent = malloc(verticesCount * sizeof(*parent));
    TPQueue* pQueue = CreateEmptyPQueue(verticesCount);
    Enqueue(pQueue, (TPair){.Key=0, .Value=0});
    for (int i = 1; i < neighboursCount[0]; ++i) {
        Enqueue(pQueue, (TPair){.Key=PQUEUE_INF_KEY, .Value=i});
    }

    while (!IsEmpty(pQueue)) {
        int vertex = Dequeue(pQueue);
    }

    return NULL;
}

int main() {
    int n, m;
    if (scanf("%d", &n) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (n < 0 || n > 5000) {
        printf("bad number of vertices\n");
        return 0;
    }
    if (scanf("%d", &m) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (m < 0 || m > n * (n - 1) / 2) {
        printf("bad number of edges\n");
        return 0;
    }

    TGraph* graph = CreateEmptyGraph(n);
    assert(graph);
    short** neighbours = graph->Neighbours;
    int** weights = graph->Weights;
    short* neighboursCount = graph->NeighboursCount;

    for (int i = 0; i < m; ++i) {
        short int from, to;
        int weight;
        if (scanf("%hd %hd %d", &from, &to, &weight) != 3) {
            printf("bad number of lines\n");
            DeleteGraph(graph);
            return 0;
        }
        if (to > n || from > n || to <= 0 || from <= 0) {
            printf("bad vertex\n");
            DeleteGraph(graph);
            return 0;
        }
        if (weight < 0) {
            printf("bad length\n");
            DeleteGraph(graph);
            return 0;
        }
        neighboursCount[from - 1]++;
        neighboursCount[to - 1]++;
    }
    for (int i = 0; i < n; ++i) {
        neighbours[i] = malloc(sizeof(short) * neighboursCount[i]);
        weights[i] = malloc(sizeof(int) * neighboursCount[i]);
    }

    FILE* in = fopen("in.txt", "r");
    SkipNum(in, 2);
    short* neighboursCountCopy = calloc(n, sizeof(*neighboursCountCopy));
    if (neighboursCountCopy == NULL) {
        DeleteGraph(graph);
        return 0;
    }
    for (int i = 0; i < m; ++i) {
        short int from, to;
        int weight;
        if (fscanf(in, "%hd %hd %d", &from, &to, &weight) != 3) {
            printf("bad number of lines\n");
            DeleteGraph(graph);
            return 0;
        }
        from--;
        to--;
        int fromIdx = neighboursCountCopy[from];
        int toIdx = neighboursCountCopy[to];
        neighbours[from][fromIdx] = to;
        neighbours[to][toIdx] = from;
        weights[from][fromIdx] = weight;
        weights[to][toIdx] = weight;
        neighboursCountCopy[from]++;
        neighboursCountCopy[to]++;
    }
    free(neighboursCountCopy);

    DeleteGraph(graph);
    
    return 0;
}
