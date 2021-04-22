#include "DisjointSetUnion.h"
#include <malloc.h>

TDSU* MakeSets(int count) {
    TDSU* dsu = malloc(sizeof(TDSU));
    if (dsu == NULL) {
        return NULL;
    }
    dsu->Parent = calloc(count, sizeof(short int));
    dsu->Rank = calloc(count, sizeof(short int));
   if (dsu->Parent == NULL || dsu->Rank == NULL) {
        DeleteSets(dsu);
        return NULL;
    }
    for (int i = 0; i < count; ++i) {
        dsu->Parent[i] = i;
    }
    return dsu;
}

void DeleteSets(TDSU* dsu) {
    if (dsu == NULL) {
        return;
    }
    free(dsu->Parent);
    free(dsu->Rank);
    free(dsu);
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
