#include "debug.h"
#include "Data.h"
#include <vector>
#include <algorithm>
#include <cstdlib>

bool verificaConstrucao(Data &data, Solution &s)
{
    bool c1 = false;
    bool c2 = false;
    bool c3 = true;


    if (s.sequence.size() == data.getDimension() + 1)
    {
        c1 = true;
    }

    int last = data.getDimension();

 

    if (s.sequence[0] == 1 && s.sequence[last] == 1)
    {
        c2 = true;
    }

    vector<bool> teste(data.getDimension(), false);

    for (int n = 0; n < data.getDimension(); n++)
    {
        int number = s.sequence[n];
        if (teste[number] == true)
        {
            c3 = false;
        }
        else
        {
            teste[number] = true;
        }
    }

    
    return c1 == true && c2 == true && c3 == true;
}


bool verificaSwap(Data &data, Solution s, int j, int i, double delta)
{
    int vi = 0;
    int k = 0;
    s.valorobj += delta;


    swap(s.sequence[i], s.sequence[j]);
    for (k = 0; k <= s.sequence.size() - 1; k++)
    {
        cout << s.sequence[k] << "->";
    }

    return verificaValorObj(s, data);

}

bool verificaTwoopt(Data &data, Solution s, int i, int j, double delta)
{
    int vi = 0;
    int k = 0;
    s.valorobj += delta;

    reverse(s.sequence.begin() + i, s.sequence.begin() + j + 1);

    for (k = 0; k <= s.sequence.size() - 1; k++)
    {
        cout << s.sequence[k] << "->";
    }

    return verificaValorObj(s, data);


}

bool verificaOrOpt(Data &data, Solution s, int i, int j, int n, double delta)
{
    int vi = 0;
    int k = 0;
    s.valorobj += delta;

    double Teste = 0;
    double TesteAux = 0;

    if (j < i)
    {
        rotate(s.sequence.begin() + j + 1, s.sequence.begin() + i, s.sequence.begin() + (i + n));
    }
    else
    {
        rotate(s.sequence.begin() + i, s.sequence.begin() + i + n, s.sequence.begin() + j + 1);
    }

    return verificaValorObj(s, data);
}

bool verificaValorObj(Solution &s, Data &data)
{
    double aux = 0;
    double cost = 0;
    double acumulado = 0;
    double antigo = 0;
    int i = 0;
    int j = 0;
    int vi, vj;
    for (i = 0; i < s.sequence.size() - 1; i++)
    {
        j = i + 1;
        vi = s.sequence[i];
        vj = s.sequence[j];
        aux = data.getDistance(vi, vj);
        antigo = cost;
        cost = antigo + acumulado + aux;
        acumulado = cost - antigo;
    }

    return cost == s.valorobj;
}
