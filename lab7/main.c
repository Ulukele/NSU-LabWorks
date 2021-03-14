#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

// Compressed Row Storage
struct CRS {
    int* Rows;
    short int* Columns;
};
typedef struct CRS TMatrix;

// Store all DFS data
typedef struct DFS {
    TMatrix* Graph;
    short int* Answer;
    int* AnswerLen;
    short int* Color;
} DFS;

TMatrix* CreateMatrix(int n, int m) {
    short int* columns = malloc(sizeof(*columns) * m);

    int* rows = malloc(sizeof(*rows) * (n + 1));
    for (int i = 0; i < n + 1; ++i) {
        rows[i] = 0;
    }

    TMatrix* matrix = malloc(sizeof(*matrix));
    matrix->Rows = rows;
    matrix->Columns = columns;

    return matrix;
}

void DeleteMatrix(TMatrix* matrix) {
    free(matrix->Columns);
    free(matrix->Rows);
    free(matrix);
}

void Push(short int* array, int* last, short int value) {
    array[*last] = value;
    if (*last > 0) {
        (*last)--;
    }
}

bool Dfs(DFS* dfs, int v) {

    short int* answer = dfs->Answer;
    int* ans_last = dfs->AnswerLen;
    short int* color = dfs->Color;
    TMatrix* graph = dfs->Graph;
    const int* rows = graph->Rows;
    const short int* columns = graph->Columns;

    color[v] = 1;
    for (int i = rows[v]; i < rows[v + 1]; ++i) {
        int to = (int)columns[i];
        if (color[to] == 1) {
            return false;
        }
        if (color[to] == 0) {
            bool status = Dfs(dfs, to);
            if (!status) {
                return false;
            }
        }
    }
    color[v] = 2;
    Push(answer, ans_last, (short int)(v + 1));
    return true;
}

bool TopologicalSort(int n, TMatrix* graph, short int* answer) {
    short int* color = malloc(sizeof(*color) * n);
    int ans_last = n - 1;

    for (int i = 0; i < n; ++i) {
        color[i] = 0;
    }

    DFS dfs = (DFS){graph, answer, &ans_last, color};

    for (int i = 0; i < n; ++i) {
        if (color[i] == 0) {
            bool status = Dfs(&dfs, i);
            if (!status) {
                free(color);
                return false;
            }
        }
    }
    free(color);
    return true;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (n < 0 || n > 2000) {
        printf("bad number of vertices\n");
        return 0;
    }

    int m;
    if (scanf("%d", &m) != 1) {
        printf("bad number of lines\n");
        return 0;
    }
    if (m < 0 || m > n * (n - 1) / 2) {
        printf("bad number of edges\n");
        return 0;
    }

    TMatrix* graph = CreateMatrix(n, m);
    short int* columns = graph->Columns;
    int* rows = graph->Rows;

    // Count edges for each vertex and check input
    for (int i = 0; i < m; ++i) {
        int u, v;
        if (scanf("%d %d", &u, &v) != 2) {
            printf("bad number of lines\n");
            DeleteMatrix(graph);
            return 0;
        }
        if (u < 1 || v < 1 || u > n || v > n) {
            printf("bad vertex\n");
            DeleteMatrix(graph);
            return 0;
        }
        rows[u]++;
    }

    FILE* file = fopen("in.txt", "r");
    if (fscanf(file, "%d %d", &n, &m) != 2) { // Read n, m again
        printf("bad number of lines\n");
        fclose(file);
        DeleteMatrix(graph);
        return 0;
    }

    // To insert edges to CSR Matrix remember last available place for each vertex
    int* rows_copy = malloc(sizeof(*rows_copy) * (n + 1));
    for (int i = 1; i < n + 1; ++i) {
        rows[i] += rows[i - 1];
        rows_copy[i] = rows[i];
    }

    for (int i = 0; i < m; ++i) { // Input was checked above
        int u = 0, v = 0;
        if (fscanf(file,"%d %d", &u, &v) != 2) {
            printf("bad number of lines\n");
            fclose(file);
            free(rows_copy);
            DeleteMatrix(graph);
            return 0;
        }

        int index = rows_copy[u] - 1;
        rows_copy[u]--;
        columns[index] = (short int)(v - 1);
    }
    fclose(file);
    free(rows_copy);

    short int* answer = malloc(sizeof(*answer) * n);
    bool possible = TopologicalSort(n, graph, answer);

    if (possible) {
        for (int i = 0; i < n; ++i) {
            printf("%d ", answer[i]);
        }
    }
    else {
        printf("impossible to sort\n");
    }
    free(answer);
    DeleteMatrix(graph);
    printf("\n");
    return 0;
}
