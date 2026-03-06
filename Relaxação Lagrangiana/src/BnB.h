#ifndef BNB_H
#define BNB_H

#include "relaxacao_funcoes.h"
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
    vector<int> highestDegreeAdj;
    int highestDegreeNode;
    double lower_bound; // custo total da solucao do algoritmo hungaro
    bool feasible;      // indica se a solucao do AP_TSP é viavel
    vector<double> lambda;

    bool operator<(const Node &next) const
    {
        return lower_bound > next.lower_bound;
    }
};

struct AuxNode
{
    vector<int> degreeAdj;
    int v;
    double cost;
    bool tour;
    vector<double> lambda;
};

AuxNode verificaTour(Data *data, vector<double> &lambda, double upper_bound, double **cost, const vector<pair<int, int>> &forbidden_arcs);
double branch_and_bound(Data *data, double upper_bound, int tipo);
void printNo(long long nodeCount, long long left, long long treeSz, double bestInteger, double bestBound, long long iterCount, double gap, vector<pair<int, int>> &forbidden_arcs);
void updateNode(Node *node, Data *data, double &upper_bound, double **cost, vector<double> &lambda);
bool isTour(const vector<vector<int>> &adj);

#endif