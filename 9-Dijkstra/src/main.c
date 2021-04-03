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
    int verticesCount = graph->VerticesCount;
    short** neighbours = graph->Neighbours;
    int** weights = graph->Weights;
    short* neighboursCount = graph->NeighboursCount;

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
        for (int i = 0; i < neighboursCount[v]; ++i) {
            int u = neighbours[v][i];
            if (order[u] == -1) {
                keys[u] = weights[v][i];
                parents[u] = v;
                shortestLens[u] = shortestLens[v] + weights[v][i];
                Enqueue(pQueue, u);
            } else if (weights[v][i] < keys[u] && !visited[u]) {
                parents[u] = v;
                shortestLens[u] = shortestLens[v] + weights[v][i];
                DecreaseKey(pQueue, u, weights[v][i]);
            }
            if (shortestLens[u] < 0 || shortestLens[parents[u]] == -1) {
                shortestLens[u] = -1;
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
    SkipNum(in, 4);
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
    for (int i = 0; i < neighboursCount[f]; ++i) {
        int u = neighbours[f][i];
        if (parents[u] != -1) {
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
