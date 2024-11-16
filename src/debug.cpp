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

    /* cout << "Tamanho de s " << s.sequence.size() << endl;
    cout << "Tamanho da dimensão " << data.getDimension() << endl; */

    if (s.sequence.size() == data.getDimension() + 1)
    {
        c1 = true;
    }

    int last = data.getDimension();

    /*    cout << "Primeiro " << s.sequence[0] << endl;
       cout << "ultimo " << s.sequence[last] << endl; */

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
    /*
        cout << c1 << endl;
        cout << c2 << endl;
        cout << c3 << endl; */
    cout << "Ação bem sucedida" << endl;

    return c1 == true && c2 == true && c3 == true;
}

bool verificaValorDelta(Data &data, Solution &s, double delta)
{
    double deltaTeste = 0;
    double deltaAux = 0;

    for (int i = 0; i < s.sequence.size() - 1; i++)
    {
        int vi = s.sequence[i];
        int vj = s.sequence[i + 1];

        deltaAux = data.getDistance(vi, vj);

        deltaTeste = deltaAux + deltaTeste;
    }

    cout << "Delta: " << delta << endl;
    cout << "DeltaTeste: " << deltaTeste << endl;

    if (delta == deltaTeste)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool verificamovimento(Data &data, Solution s, int j, int i, int n, double delta)
{
    int vi = 0;
    int k = 0;
    double valor = s.valorobj + delta;
    if (j < i)
    {
        rotate(s.sequence.begin() + j + 1, s.sequence.begin() + i, s.sequence.begin() + (i + n));
    }
    else
    {
        rotate(s.sequence.begin() + i, s.sequence.begin() + i + n, s.sequence.begin() + j + 1);
    }

    double Teste = 0;
    double TesteAux = 0;

    for (int i = 0; i < s.sequence.size() - 1; i++)
    {
        int vi = s.sequence[i];
        int vj = s.sequence[i + 1];

        TesteAux = data.getDistance(vi, vj);

        Teste = TesteAux + Teste;
    }

    cout << "Sequência: ";
    for (i = 0; i < s.sequence.size() - 1; i++)
    {
        cout << s.sequence[i] << " -> ";
    }

    cout << s.sequence.back();

    cout << endl;
    cout << "Valor desse movimento: " << Teste;

    if (Teste == valor)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool verificaSwap(Data &data, Solution s, int j, int i, double delta)
{
    int vi = 0;
    int k = 0;
    double valor = s.valorobj + delta;

    double Teste = 0;
    double TesteAux = 0;

    swap(s.sequence[i], s.sequence[j]);
    for (k = 0; k <= s.sequence.size() - 1; k++)
    {
        cout << s.sequence[k] << "->";
    }

    cout << endl;

    for (int i = 0; i < s.sequence.size() - 1; i++)
    {
        int vi = s.sequence[i];
        int vj = s.sequence[i + 1];

        TesteAux = data.getDistance(vi, vj);

        Teste = TesteAux + Teste;
    }

    cout << "Valor desse movimento: " << Teste;
    cout << endl;
    cout << endl;

    if (Teste == valor)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool verificaTwoopt(Data &data, Solution s, int i, int j, double delta)
{
    int vi = 0;
    int k = 0;
    cout << "I: " << i << endl;
    cout << "J: " << j << endl;
    double valor = s.valorobj + delta;

    double Teste = 0;
    double TesteAux = 0;

    reverse(s.sequence.begin() + i, s.sequence.begin() + j + 1);

    for (k = 0; k <= s.sequence.size() - 1; k++)
    {
        cout << s.sequence[k] << "->";
    }

    cout << endl;

    for (int i = 0; i < s.sequence.size() - 1; i++)
    {
        int vi = s.sequence[i];
        int vj = s.sequence[i + 1];

        TesteAux = data.getDistance(vi, vj);

        Teste = TesteAux + Teste;
    }

    cout << "Valor desse movimento: " << Teste;
    cout << endl;

    if (Teste == valor)
    {
        return true;
    }
    else
    {
        return false;
    }
}
