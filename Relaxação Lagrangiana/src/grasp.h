#ifndef GRASP_H
#define GRASP_H

#include <vector>
#include "Data.h"

using namespace std;

struct InsertionInfo
{
    int noInserido;
    int arestaRemovida;
    double custo;
};

struct Solution
{
    vector<int> sequence;
    double valorobj;
};

double grasp(Data &data);
Solution ILS(int maxIter, int maxIterIls, Data &data);
void buscaLocal(Solution &s, Data &data);
bool orOpt(Solution &s, Data &data, int n);
bool twoOpt(Solution &s, Data &data);
bool Swap(Solution &s, Data &data);
Solution Construcao(Data &data);
bool compararCusto(InsertionInfo &a, InsertionInfo &b);
vector<int> escolher3NosAleatorios(Solution &s, vector<int> &CL);
void calcularValorObj(Solution &s, Data &data);
vector<InsertionInfo> calcularCusto(Solution &s, vector<int> &CL, Data &data);

#endif