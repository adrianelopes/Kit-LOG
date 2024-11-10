#ifndef DEBUG_H
#define DEBUG_H
#include "Data.h"
#include <vector>
struct Solution
{
    vector<int> sequence;
    double valorobj;
};
bool verificaConstrucao(Data &data, Solution &s);
bool verificaValorDelta(Data &data, Solution &s, double delta);

#endif
