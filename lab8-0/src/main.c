#include <stdio.h>
#include <malloc.h>

typedef struct {
    int Begin;
    int End;
    int Weight;
} TEdge;

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

    TEdge* edges = malloc(sizeof(*edges) * m);
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
    }


    return 0;
}
