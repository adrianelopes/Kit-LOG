#ifndef DEBUG_H
#define DEBUG_H
#include "Data.h"
#include <vector>
struct Solution
{
    vector<int> sequence;
    double valorobj;
    double custoAcumulado;
};
bool verificaConstrucao(Data &data, Solution &s);
bool verificaValorDelta(Data &data, Solution &s, double delta);
bool verificamovimento(Data &data, Solution s, int j, int i, int n, double delta);
bool verificaSwap(Data &data, Solution s, int j, int i, double delta);
bool verificaTwoopt(Data &data, Solution s, int i, int j, double delta);
bool verificaValorObj(Solution &s, Data &data);

#endif
