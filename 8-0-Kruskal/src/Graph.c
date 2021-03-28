#include "Graph.h"
#include <stdlib.h>

static int CompareEdges(const TEdge* first, const TEdge* second) {
    return first->Weight - second->Weight;
}

void SortEdges(TEdge* edges, int edgesCount) {
    qsort(edges, edgesCount, sizeof(TEdge), (int(*) (const void*, const void*))CompareEdges);
}
