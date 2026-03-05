#ifndef BNB_H
#define BNB_H

#include "Data.h"
#include "relaxacao_lagrangiana.h"
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

struct Node
{
    vector<pair<int, int>> forbidden_arcs;
    vector<int> smallersubtour;
    double lower_bound; // custo total da solucao do algoritmo hungaro
    bool feasible;      // indica se a solucao do AP_TSP é viavel

    bool operator<(const Node &next) const
    {
        return lower_bound > next.lower_bound;
    }
};

void updateNode(Node *node, Data *data, double **cost);
double branch_and_bound(Data *data, double upper_bound, int tipo);
void printNo(long long nodeCount, long long left, long long treeSz, double bestInteger, double bestBound, long long iterCount, double gap);

#endif