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

vector<vector<double>> modifica_Matriz(Data *data);
// Tree Make1_Tree_Original(Data &data);
vector<vector<double>> altera_matriz(Data *data, vector<double> lambda, vector<vector<double>> matriz);
Tree Make1_Tree(Data *data, vector<double> lambda);
Tree Subgradient(Data *data, double UB, vector<double> lambda);

#endif