#pragma once

typedef struct {
    short int* Parent;
    short int* Rank;
} TDSU;

TDSU* MakeSets(int count);

void DeleteSets(TDSU* dsu);

int FindSet(TDSU* dsu, int element);

void MergeSets(TDSU* dsu, int firstSet, int secondSet);
