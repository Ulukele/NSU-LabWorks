#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include "Graph.h"
#include "PriorityQueue.h"

void SkipNum(FILE* file, int count) {
    int skip;
    for (int i = 0; i < count; ++i) {
        if (fscanf(file, "%d", &skip) != 1) {
            return;
        }
    }
}

TEdgeLight* FindSpanningTree(TGraph* graph) {
    int verticesCount = graph->VerticesCount;
    short** neighbours = graph->Neighbours;
    int** weights = graph->Weights;
    short* neighboursCount = graph->NeighboursCount;

    if (verticesCount == 0) {
        return NULL;
    }

    TEdgeLight* spanningTree = malloc((verticesCount - 1) * sizeof(*spanningTree));
    int spanningTreeLen = 0;
    short* parents = malloc(verticesCount * sizeof(*parents));
    int* keys = malloc(verticesCount * sizeof(*keys));
    short* order = malloc(verticesCount * sizeof(*order));
    bool* visited = malloc(verticesCount * sizeof(*visited));
    TPQueue* pQueue = CreateEmptyPQueue(verticesCount, keys, order);

    if (!spanningTree || !parents || !keys || !order || !visited || !pQueue) {
        free(spanningTree);
        free(parents);
        free(keys);
        free(order);
        free(visited);
        DeletePQueue(pQueue);
        return NULL;
    }

    for (int i = 0; i < verticesCount; ++i) {
        parents[i] = -1;
        keys[i] = -1;
        order[i] = -1;
        visited[i] = false;
    }
    keys[0] = 0;
    Enqueue(pQueue, 0);

    while (!IsEmpty(pQueue)) {
        int v = Dequeue(pQueue);
        visited[v] = true;
        int parent = parents[v];
        for (int i = 0; i < neighboursCount[v]; ++i) {
            int u = neighbours[v][i];
            if (order[u] == -1) {
                keys[u] = weights[v][i];
                parents[u] = v;
                Enqueue(pQueue, u);
            } else if (weights[v][i] < keys[u] && !visited[u]) {
                parents[u] = v;
                DecreaseKey(pQueue, u, weights[v][i]);
            }
        }
        if (parent != -1) {
            spanningTree[spanningTreeLen].Begin = v;
            spanningTree[spanningTreeLen].End = parent;
            spanningTreeLen++;
        }
    }
    free(parents);
    free(keys);
    free(order);
    free(visited);
    DeletePQueue(pQueue);
    if (spanningTreeLen != verticesCount - 1) {
        free(spanningTree);
        return NULL;
    }
    return spanningTree;
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
        short from, to;
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
        short from, to;
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
    fclose(in);

    TEdgeLight* spanningTree = FindSpanningTree(graph);
    if (spanningTree == NULL) {
        printf("no spanning tree\n");
    }
    else {
        for (int i = 0; i < n - 1; ++i) {
            printf("%d %d\n", spanningTree[i].Begin + 1, spanningTree[i].End + 1);
        }
    }
    free(spanningTree);
    DeleteGraph(graph);
    
    return 0;
}
