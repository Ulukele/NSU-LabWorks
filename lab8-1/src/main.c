#include <stdio.h>
#include <malloc.h>
#include "Graph.h"

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

    short** neighbours = malloc(sizeof(short*) * n);
    short* neighboursCount = calloc(n, sizeof(*neighboursCount));
    for (int i = 0; i < m; ++i) {
        short int from, to;
        int weight;
        if (scanf("%hd %hd %d", &from, &to, &weight) != 3) {
            printf("bad number of lines\n");
            free(neighboursCount);
            free(neighbours);
            return 0;
        }
        if (to > n || from > n || to <= 0 || from <= 0) {
            printf("bad vertex\n");
            free(neighboursCount);
            free(neighbours);
            return 0;
        }
        if (weight < 0) {
            printf("bad length\n");
            free(neighboursCount);
            free(neighbours);
            return 0;
        }
        neighboursCount[from - 1]++;
        neighboursCount[to - 1]++;
    }
    for (int i = 0; i < n; ++i) {
        neighbours[i] = malloc(sizeof(short) * neighboursCount[i]);
    }

    TGraph graph;
    graph.Neighbours = neighbours;
    graph.NeighboursCount = neighboursCount;
    graph.VerticesCount = n;
    
    for (int i = 0; i < n; ++i) {
        free(neighbours[i]);
    }
    free(neighbours);
    free(neighboursCount);
    
    return 0;
}
