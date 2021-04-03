#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include "Graph.h"
#include "PriorityQueue.h"
#include "MemManager.h"

void SkipNum(FILE* file, int count) {
    int skip;
    for (int i = 0; i < count; ++i) {
        if (fscanf(file, "%d", &skip) != 1) {
            return;
        }
    }
}

TEdgeLight* FindSpanningTree(TGraph* graph) {
    TMemNode* cleaner = InitCleaner();
    int verticesCount = graph->VerticesCount;
    short** neighbours = graph->Neighbours;
    int** weights = graph->Weights;
    short* neighboursCount = graph->NeighboursCount;

    if (verticesCount == 0) {
        CleanUp(cleaner);
        return NULL;
    }

    TEdgeLight* spanningTree = malloc((verticesCount - 1) * sizeof(*spanningTree));
    int spanningTreeLen = 0;
    short* parents = MallocAuto(cleaner, verticesCount * sizeof(*parents));
    int* keys = MallocAuto(cleaner, verticesCount * sizeof(*keys));
    short* order = MallocAuto(cleaner, verticesCount * sizeof(*order));
    bool* visited = MallocAuto(cleaner, verticesCount * sizeof(*visited));
    TPQueue* pQueue = CreateEmptyPQueue(verticesCount, keys, order);

    if (!spanningTree || !parents || !keys || !order || !visited || !pQueue) {
        CleanUp(cleaner);
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
    CleanUp(cleaner);
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

    TMemNode* graphCleaner = InitCleaner();
    TGraph* graph = CreateEmptyGraph(n, graphCleaner);
    assert(graph);
    short** neighbours = graph->Neighbours;
    int** weights = graph->Weights;
    short* neighboursCount = graph->NeighboursCount;

    for (int i = 0; i < m; ++i) {
        short from, to;
        int weight;
        if (scanf("%hd %hd %d", &from, &to, &weight) != 3) {
            printf("bad number of lines\n");
            CleanUp(graphCleaner);
            return 0;
        }
        if (to > n || from > n || to <= 0 || from <= 0) {
            printf("bad vertex\n");
            CleanUp(graphCleaner);
            return 0;
        }
        if (weight < 0) {
            printf("bad length\n");
            CleanUp(graphCleaner);
            return 0;
        }
        neighboursCount[from - 1]++;
        neighboursCount[to - 1]++;
    }
    AllocateNeighbours(graph, graphCleaner);

    FILE* in = fopen("in.txt", "r");
    SkipNum(in, 2);
    short* neighboursCountCopy = calloc(n, sizeof(*neighboursCountCopy));
    if (neighboursCountCopy == NULL) {
        fclose(in);
        CleanUp(graphCleaner);
        return 0;
    }
    for (int i = 0; i < m; ++i) {
        short from, to;
        int weight;
        if (fscanf(in, "%hd %hd %d", &from, &to, &weight) != 3) {
            printf("bad number of lines\n");
            fclose(in);
            free(neighboursCountCopy);
            CleanUp(graphCleaner);
            return 0;
        }
        from--;
        to--;
        
        int fromIdx = neighboursCountCopy[from];
        neighbours[from][fromIdx] = to;
        weights[from][fromIdx] = weight;
        neighboursCountCopy[from]++;

        int toIdx = neighboursCountCopy[to];
        neighbours[to][toIdx] = from;
        weights[to][toIdx] = weight;
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
    CleanUp(graphCleaner);
    
    return 0;
}
