#ifndef RELAXACAO_H
#define RELAXACAO_H
#include <iostream>
#include <vector>
#include "Kruskal.h"
#include "Data.h"
#include <limits>
#include <iomanip>

using namespace std;

struct Tree
{
    double cost;
    vector<vector<int>> listAdj;
    vector<double> lambda;
};

vector<vector<double>> modifica_Matriz(Data *data, const vector<pair<int, int>> &forbidden_arcs);
// Tree Make1_Tree_Original(Data &data);
void altera_matriz(Data *data, double **cost, vector<double> &lambda, vector<vector<double>> &matriz);
Tree Make1_Tree(Data *data, double **cost, vector<double> &lambda, const vector<pair<int, int>> &forbidden_arcs);
Tree Subgradient(Data *data, double **cost, double UB, vector<double> &lambda, const vector<pair<int, int>> &forbidden_arcs);

#endif