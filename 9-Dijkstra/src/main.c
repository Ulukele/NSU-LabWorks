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

void FindPaths(TGraph* graph, short s, int* shortestLens, short* parents) {
    TMemNode* cleaner = InitCleaner();
    int* weightsMatrix = graph->WeightsMatrix;
    int verticesCount = graph->VerticesCount;

    if (verticesCount == 0) {
        CleanUp(cleaner);
        return;
    }
    
    int* keys = MallocAuto(cleaner, verticesCount * sizeof(*keys));
    short* order = MallocAuto(cleaner, verticesCount * sizeof(*order));
    bool* visited = MallocAuto(cleaner, verticesCount * sizeof(*visited));
    TPQueue* pQueue = CreateEmptyPQueue(verticesCount, keys, order);

    if (!keys || !order || !visited || !pQueue) {
        CleanUp(cleaner);
        DeletePQueue(pQueue);
        return;
    }

    for (int i = 0; i < verticesCount; ++i) {
        parents[i] = -1;
        keys[i] = -1;
        order[i] = -1;
        visited[i] = false;
        shortestLens[i] = -2;
    }
    parents[s] = s;
    keys[s] = 0;
    shortestLens[s] = 0;
    Enqueue(pQueue, s);

    while (!IsEmpty(pQueue)) {
        int v = Dequeue(pQueue);
        visited[v] = true;
        for (int i = 0; i < verticesCount; ++i) {
            if (weightsMatrix[v * verticesCount + i] == -1) {
                continue;
            }
            if (order[i] == -1) {
                keys[i] = weightsMatrix[v * verticesCount + i];
                parents[i] = v;
                shortestLens[i] = shortestLens[v] + weightsMatrix[v * verticesCount + i];
                Enqueue(pQueue, i);
            } else if (weightsMatrix[v * verticesCount + i] < keys[i] && !visited[i]) {
                parents[i] = v;
                shortestLens[i] = shortestLens[v] + weightsMatrix[v * verticesCount +i];
                DecreaseKey(pQueue, i, weightsMatrix[v * verticesCount + i]);
            }
            if (shortestLens[i] < 0 || shortestLens[parents[i]] == -1) {
                shortestLens[i] = -1;
            }
        }
    }
    CleanUp(cleaner);
    DeletePQueue(pQueue);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (n < 0 || n > 5000) {
        printf("bad number of vertices\n");
        return 0;
    }

    short s, f;
    if (scanf("%hd %hd", &s, &f) != 2) {
        printf("bad number of lines\n");
        return 0;
    }
    if (f <= 0 || f > n || s <= 0 || s > n) {
        printf("bad vertex\n");
        return 0;
    }
    s--;
    f--;

    int m;
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
    int* weightsMatrix = graph->WeightsMatrix;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            weightsMatrix[i * n + j] = -1;
        }
    } 

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
        from--;
        to--;
        weightsMatrix[from * n + to] = weight;
        weightsMatrix[to * n + from] = weight;
    }

    TMemNode* cleaner = InitCleaner();
    int* shortestLens = MallocAuto(cleaner, n * sizeof(*shortestLens));
    short* parents = MallocAuto(cleaner, n * sizeof(*parents));
    FindPaths(graph, s, shortestLens, parents);
    
    for (int i = 0; i < n; ++i) {
        if (shortestLens[i] >= 0) {
            printf("%d ", shortestLens[i]);
        }
        else if (shortestLens[i] == -1) {
            printf("INT_MAX+ ");
        }
        else {
            printf("oo ");
        }
    }
    printf("\n");

    int ways = (f == s);
    for (int i = 0; i < n; ++i) {
        if (weightsMatrix[f * n + i] == -1) {
            continue;
        }
        if (parents[i] != -1) {
            ways++;
        }
    }
    if (shortestLens[f] == -1 && ways > 1) {
        printf("overflow");
    }
    else if (ways > 0) {
        int step = f;
        while (step != s) {
            printf("%hd ", step + 1);
            step = parents[step];
        }
        printf("%hd", s + 1);
    }
    else {
        printf("no path");
    }

    CleanUp(cleaner);
    CleanUp(graphCleaner);
    
    return 0;
}
