#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

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

typedef struct {
    short int* Parent;
    short int* Rank;
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

TEdgeLight* FindSpanningTree(TGraph* graph) {
    TEdge* edges = graph->Edges;
    int edgesCount = graph->EdgesCount;
    int verticesCount = graph->VerticesCount;
    if (verticesCount == 0) {
        return NULL;
    }

    int spanningTreeLen = 0;
    TEdgeLight* spanningTree = malloc((verticesCount - 1) * sizeof(*spanningTree));
    if (spanningTree == NULL) {
        return NULL;
    }

    SortEdges(edges, edgesCount);
    TDSU* dsu = MakeSets(verticesCount);
    for (int i = 0; i < edgesCount; ++i) {
        short int from = edges[i].Begin;
        short int to = edges[i].End;
        if (FindSet(dsu, from) != FindSet(dsu, to)) {
            MergeSets(dsu, from, to);
            spanningTree[spanningTreeLen].Begin = edges[i].Begin;
            spanningTree[spanningTreeLen].End = edges[i].End;
            spanningTreeLen++;
        }
    }

    bool haveSpanningTree = true;
    for (int i = 0; i < verticesCount - 1; ++i) {
        if (FindSet(dsu, i) != FindSet(dsu, i + 1)) {
            haveSpanningTree = false;
            break;
        }
    }
    DeleteSets(dsu);
    if (haveSpanningTree) {
        return spanningTree;
    }
    else {
        free(spanningTree);
        return NULL;
    }
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
    TEdge* edges = malloc(sizeof(*edges) * m);
    if (edges == NULL && m != 0) {
        return 0;
    }
    for (int i = 0; i < m; ++i) {
        short int from, to;
        int weight;
        if (scanf("%hd %hd %d", &from, &to, &weight) != 3) {
            printf("bad number of lines\n");
            free(edges);
            return 0;
        }
        if (to > n || from > n || to <= 0 || from <= 0) {
            printf("bad vertex\n");
            free(edges);
            return 0;
        }
        if (weight < 0) {
            printf("bad length\n");
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
    
    TEdgeLight* spanningTree = FindSpanningTree(&graph);
    if (spanningTree == NULL) {
        free(edges);
        printf("no spanning tree\n");
        return 0;
    }

    for (int i = 0; i < n - 1; ++i) {
        printf("%d %d\n", spanningTree[i].Begin + 1, spanningTree[i].End + 1);
    }

    free(edges);
    free(spanningTree);
    
    return 0;
}
