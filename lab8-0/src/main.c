#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct {
    int Begin;
    int End;
    int Weight;
} TEdge;

typedef struct {
    TEdge* Edges;
    int VerticesCount;
    int EdgesCount;
} TGraph;

typedef struct {
    int* Parent;
    int* Rank;
} TDSU;

void DeleteSets(TDSU* dsu) {
    if (dsu == NULL) {
        return;
    }
    free(dsu->Parent);
    free(dsu->Rank);
    free(dsu);
}

TDSU* MakeSets(int count) {
    TDSU* dsu = malloc(sizeof(TDSU));
    if (dsu == NULL) {
        return NULL;
    }
    dsu->Parent = calloc(count, sizeof(int));
    dsu->Rank = calloc(count, sizeof(int));
   if (dsu->Parent == NULL || dsu->Rank == NULL) {
        DeleteSets(dsu);
        return NULL;
    }
    for (int i = 0; i < count; ++i) {
        dsu->Parent[i] = i;
    }
    return dsu;
}

int FindSet(TDSU* dsu, int element) {
    if (element == dsu->Parent[element]) {
        return element;
    }
    dsu->Parent[element] = FindSet(dsu, dsu->Parent[element]);
    return dsu->Parent[element];
}

void MergeSets(TDSU* dsu, int firstSet, int secondSet) {
    firstSet = FindSet(dsu, firstSet);
    secondSet = FindSet(dsu, secondSet);
    if (dsu->Rank[firstSet] > dsu->Rank[secondSet]) {
        dsu->Parent[secondSet] = firstSet;
        return;
    }
    dsu->Parent[firstSet] = secondSet;
    if (dsu->Rank[firstSet] == dsu->Rank[secondSet]) {
        dsu->Rank[secondSet] += 1;
    }
}

int CompareEdges(const TEdge* first, const TEdge* second) {
    return first->Weight - second->Weight;
}

void SortEdges(TEdge* edges, int edgesCount) {
    qsort(edges, edgesCount, sizeof(TEdge), (int(*) (const void*, const void*))CompareEdges);
}

TEdge* FindSpanningTree(TGraph* graph) {
    TEdge* edges = graph->Edges;
    int edgesCount = graph->EdgesCount;
    int verticesCount = graph->VerticesCount;

    TEdge* spanningTree = calloc(verticesCount - 1, sizeof(*spanningTree));
    int spanningTreeLen = 0;
    if (spanningTree == NULL) {
        return spanningTree;
    }

    SortEdges(edges, edgesCount);
    TDSU* dsu = MakeSets(verticesCount);
    for (int i = 0; i < edgesCount; ++i) {
        int from = edges[i].Begin;
        int to = edges[i].End;
        if (FindSet(dsu, from) != FindSet(dsu, to)) {
            MergeSets(dsu, from, to);
            spanningTree[spanningTreeLen] = edges[i];
            spanningTreeLen++;
        }
    }
    DeleteSets(dsu);
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

    int edgesCount = 0;
    TEdge* edges = calloc(m, sizeof(*edges));
    assert(edges);
    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        if (scanf("%d %d %d", &from, &to, &weight) != 3) {
            printf("bad number of lines\n");
            free(edges);
            return 0;
        }
        if (weight < 0) {
            printf("bad number of lines\n");
            free(edges);
            return 0;
        }
        edges[edgesCount].Begin = from - 1;
        edges[edgesCount].End = to - 1;
        edges[edgesCount].Weight = weight;
        edgesCount++;
    }

    TGraph graph;
    graph.Edges = edges;
    graph.VerticesCount = n;
    graph.EdgesCount = m;
    
    TEdge* spanningTree = FindSpanningTree(&graph);
    if (spanningTree == NULL) {
        free(edges);
        return 0;
    }

    for (int i = 0; i < n - 1; ++i) {
        printf("%d %d\n", spanningTree[i].Begin + 1, spanningTree[i].End + 1);
    }

    free(edges);
    free(spanningTree);
    
    return 0;
}
