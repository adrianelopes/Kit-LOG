#include "Data.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "debug.h"
// #define NDEBUG
#include <cassert>

using namespace std;

struct InsertionInfo
{
    int noInserido;
    int arestaRemovida;
    double custo;
};

void exibirSolucao(Solution &s)
{

    for (int i = 0; i < s.sequence.size() - 1; i++)
        std::cout << s.sequence[i] << " -> ";
    std::cout << s.sequence.back() << std::endl;
    cout << "Custo da Solução: " << s.valorobj << endl;
}

void calcularValorObj(Solution &s, Data &data)
{
    s.valorobj = 0;
    for (int i = 0; i < s.sequence.size() - 1; i++)
        s.valorobj += data.getDistance(s.sequence[i], s.sequence[i + 1]);
}

vector<InsertionInfo> calcularCusto(Solution &s, vector<int> &CL, Data &data)
{
    // número de combinações possíveis
    int n = ((s.sequence.size() - 1) * CL.size());

    vector<InsertionInfo> custoInsercao(n);
    int l = 0;
    for (int a = 0; a < s.sequence.size() - 1; a++)
    {
        int i = s.sequence[a];
        int j = s.sequence[a + 1];
        for (auto k : CL)
        {
            custoInsercao[l].custo = data.getDistance(i, k) + data.getDistance(k, j) - data.getDistance(i, j);
            custoInsercao[l].arestaRemovida = a;
            custoInsercao[l].noInserido = k;
            l++;
        }
    }

    return custoInsercao;
}

vector<int> escolher3NosAleatorios(Solution &s, vector<int> &CL)
{

    s.sequence = {1};
    for (int n = 0; n < 3; n++)
    {
        int random = rand() % (CL.size());
        int number = CL[random];
        s.sequence.push_back(number);
        CL.erase(CL.begin() + random);
    }

    s.sequence.push_back(1);

    return s.sequence;
}

bool compararCusto(InsertionInfo &a, InsertionInfo &b)
{
    return a.custo < b.custo;
}

Solution Construcao(Data &data)
{
    Solution s;
    vector<int> CL;

    // inicializando CL
    int n = data.getDimension();
    for (int a = 2; a <= n; a++)
    {
        CL.push_back(a);
    }

    s.sequence = escolher3NosAleatorios(s, CL);

    while (!CL.empty())
    {
        vector<InsertionInfo> custoInsercao = calcularCusto(s, CL, data);
        sort(custoInsercao.begin(), custoInsercao.end(), compararCusto);
        double alpha = (double)rand() / RAND_MAX;
        int selecionado = rand() % ((int)ceil(alpha * (custoInsercao.size())));
        int position = custoInsercao[selecionado].arestaRemovida + 1;
        int inserir = custoInsercao[selecionado].noInserido;
        s.sequence.insert(s.sequence.begin() + position, inserir);
        CL.erase(remove(CL.begin(), CL.end(), inserir));
    }

    calcularValorObj(s, data);
    // assert(verificaConstrucao(data, s));
    return s;
}

bool Swap(Solution &s, Data &data)
{
    double bestDelta = 0;
    int best_i, best_j;
    double delta = 0;
    int j, i;
    int q = 1;

    for (i = 1; i < s.sequence.size() - 1; i++)
    {
        int vi = s.sequence[i];
        int vin = s.sequence[i + 1];
        int vip = s.sequence[i - 1];
        j = i + 1;

        for (j; j < s.sequence.size() - 1; j++)
        {
            int vj = s.sequence[j];
            int vjn = s.sequence[j + 1];
            int vjp = s.sequence[j - 1];

            if (j == i + 1)
            {
                delta = -data.getDistance(vip, vi) - data.getDistance(vj, vjn) + data.getDistance(vip, vj) + data.getDistance(vi, vjn);
            }
            else
            {
                delta = -data.getDistance(vip, vi) - data.getDistance(vi, vin) - data.getDistance(vjp, vj) - data.getDistance(vj, vjn) + data.getDistance(vip, vj) + data.getDistance(vj, vin) + data.getDistance(vjp, vi) + data.getDistance(vi, vjn);
            }

            /*  cout << "Movimento: " << q << endl;
             assert(verificaSwap(data, s, i, j, delta));
             q++; */

            if (delta < bestDelta)
            {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (bestDelta < 0)
    {
        swap(s.sequence[best_i], s.sequence[best_j]);
        s.valorobj = s.valorobj + bestDelta;
        // assert(verificaValorDelta(data, s, s.valorobj));
        return true;
    }
    else
    {
        return false;
    }
}

bool twoOpt(Solution &s, Data &data)
{
    double bestDelta = 0;
    int best_i, best_j, i, j;
    double delta = 0;
    int q = 1;
    bool teste = false;

    for (i = 1; i < s.sequence.size() - 2; i++)
    {
        int vi = s.sequence[i];
        int vip = s.sequence[i - 1];

        for (j = i + 1; j < s.sequence.size() - 1; j++)
        {
            int vj = s.sequence[j];
            int vjn = s.sequence[j + 1];

            if (j == i + 1)
            {
                delta = -data.getDistance(vip, vi) - data.getDistance(vj, vjn) + data.getDistance(vip, vj) + data.getDistance(vi, vjn);
            }
            else
            {
                delta = -data.getDistance(vi, vip) - data.getDistance(vj, vjn) + data.getDistance(vip, vj) + data.getDistance(vi, vjn);
            }

            /* cout << "Movimento: " << q << endl;
            assert(verificaTwoopt(data, s, i, j, delta));
            q++;
            cout << "Delta calculado: " << delta << endl;
            cout << "J atual: " << j << endl; */

            if (delta < bestDelta)
            {
                /*  teste = true;
                 cout << "Teste: " << teste << endl; */
                bestDelta = delta;
                best_i = i;
                best_j = j;
                /*    cout << "Best i: " << best_i << endl;
                   cout << "Best j: " << best_j << endl; */
            }
            cout << endl;
        }
    }

    if (bestDelta < 0)
    {
        /*  cout << "Best i: " << best_i << endl;
         cout << "Best j: " << best_j << endl; */
        reverse(s.sequence.begin() + best_i, s.sequence.begin() + best_j + 1);
        s.valorobj = s.valorobj + bestDelta;
        // assert(verificaValorDelta(data, s, s.valorobj));

        return true;
    }
    else
    {
        return false;
    }
}

bool orOpt(Solution &s, Data &data, int n)
{
    double bestDelta = 0;
    double delta = 0;
    int best_i, best_j, i, j;
    int last = 0;
    int q = 1;
    // cout << "Função que chega: ";
    // exibirSolucao(s);

    for (i = 1; i < s.sequence.size() - n; i++)
    {
        int vi = s.sequence[i];
        last = i + (n - 1);
        int vin = s.sequence[last + 1];
        int vip = s.sequence[i - 1];
        int vil = s.sequence[last];

        for (j = 1; j < s.sequence.size() - 1; j++)
        {
            int vj = s.sequence[j];
            int vjp = s.sequence[j - 1];
            int vjn = s.sequence[j + 1];

            if (j == i || j == last - 1 || j == last || j == i + 1 || j == i - 1)
            {
                continue;
            }

            if (j == i - 1)
            {
                delta += +data.getDistance(vj, vi);
            }

            else
            {
                delta = -data.getDistance(vip, vi) - data.getDistance(vil, vin) - data.getDistance(vj, vjn) + data.getDistance(vip, vin) + data.getDistance(vj, vi) + data.getDistance(vil, vjn);
            }

            /* cout << "Interação: " << q << endl;
            cout << "I: " << i << endl;
            cout << "J: " << j << endl;
            assert(verificamovimento(data, s, j, i, n, delta));
            cout << endl;
            q++;
            cout << endl; */

            if (delta < bestDelta)
            {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    // cout << "Terminando o movimento" << endl;

    if (bestDelta < 0)
    {

        /*  cout << "I: " << best_i << endl;
         cout << "J: " << best_j << endl; */

        if (best_j == best_i || best_j == (best_i + (n - 1) - 1) || best_j == best_i + (n - 1) || best_j == (best_i + 1))
        {
            s.sequence = s.sequence;
            s.valorobj = s.valorobj;
            // exibirSolucao(s);
            // assert(verificaValorDelta(data, s, s.valorobj));
            return false;
        }
        else if (best_j < best_i && best_j != (best_i - 1))
        {
            rotate(s.sequence.begin() + best_j + 1, s.sequence.begin() + best_i, s.sequence.begin() + (best_i + n));
            s.valorobj = s.valorobj + bestDelta;
            // exibirSolucao(s);
            // assert(verificaValorDelta(data, s, s.valorobj));
            // cout << "rotate atras" << endl;
            return true;
            getchar();
        }
        else
        {
            rotate(s.sequence.begin() + best_i, s.sequence.begin() + best_i + n, s.sequence.begin() + best_j + 1);
            s.valorobj = s.valorobj + bestDelta;
            // exibirSolucao(s);
            // assert(verificaValorDelta(data, s, s.valorobj));
            // cout << "rotate frente" << endl;
            return true;
        }
    }
    else
    {
        return false;
    }
}

void buscaLocal(Solution &s, Data &data)
{
    vector<int> NL = {1, 2, 3, 4, 5};
    bool improved = false;

    while (NL.empty() == false)
    {
        int n = rand() % NL.size();

        switch (NL[n])
        {
        case 1:
            improved = Swap(s, data);
            break;

        case 2:
            improved = twoOpt(s, data);
            break;

        case 3:
            improved = orOpt(s, data, 1);
            break;

        case 4:
            improved = orOpt(s, data, 2);
            break;

        case 5:
            improved = orOpt(s, data, 3);
            break;
        }

        if (improved)
        {
            NL = {1, 2, 3, 4, 5};
        }
        else
        {
            NL.erase(NL.begin() + n);
        }
    }
}

Solution ILS(int maxIter, int maxIterIls, Data &data)
{
    Solution bestOfAll;
    bestOfAll.valorobj = 999999999999999;
    for (int i = 0; i < maxIter; i++)
    {
        Solution s = Construcao(data);
        Solution best = s;
        int iterIls = 0;

        while (iterIls <= maxIterIls)
        {
            buscaLocal(s, data);
            if (s.valorobj < best.valorobj)
            {
                best = s;
                iterIls = 0;
            }
            //  s = perturbacao ();
            iterIls++;
        }

        if (best.valorobj < bestOfAll.valorobj)
        {
            bestOfAll = best;
        }

        return bestOfAll;
    }
}

int main(int argc, char **argv)
{

    auto data = Data(argc, argv[1]);
    data.read();
    size_t size_i = data.getDimension();

    int maxIter = 50;
    int V = data.getDimension();
    int maxIterIls;

    if (V >= 150)
    {
        maxIterIls = (V / 2);
    }
    else
    {
        maxIterIls = V;
    }

    Solution s;

    s = ILS(maxIter, maxIterIls, data);
    exibirSolucao(s);

    srand(time(NULL));
    int k = 0;

    /*  while (k < 1)
     {
         cout << k << endl;

         s = Construcao(data);
         cout << "Solução da Construção: " << endl;
         exibirSolucao(s);
         cout << endl;

         /*  cout << "~~~~~~~~~~~~~~SWAP~~~~~~~~~~~";
          Swap(s, data);
          cout << "Solução depois do Swap: " << endl;
          exibirSolucao(s);
          cout << endl;

         twoOpt(s, data);
         cout << "Solução depois do 2OPT: " << endl;
         exibirSolucao(s);
         cout << endl;

                 cout << "~~~~~~~~~~~~~~Solução depois do orOpt-1: ~~~~~~~~~~~~~~~~~~~" << endl;
                 orOpt(s, data, 1);
                 cout << endl;
                 cout << "Solução como está: ";
                 exibirSolucao(s);
                 cout << endl;

                 cout << "~~~~~~~~~~~~~~Solução depois do orOpt-2: ~~~~~~~~~~~~~~~~~~~" << endl;
                 orOpt(s, data, 2);
                 cout << endl;
                 cout << "Solução como está: ";
                 exibirSolucao(s);
                 cout << endl;

                 cout << "~~~~~~~~~~~~~~Solução depois do orOpt-3: ~~~~~~~~~~~~~~~~~~~" << endl;
                 orOpt(s, data, 3);
                 cout << endl;
                 cout << "Solução como está: ";
                 exibirSolucao(s);
                 cout << endl;

         cout << endl;

         k++;
     }; */

    /*     cout << "Dimension: " << n << endl;
         cout << "DistanceMatrix: " << endl;
         data.printMatrixDist();

         cout << "Exemplo de Solucao s = ";
         double cost = 0.0;
         for (size_t i = 1; i < n; i++)
         {
             cout << i << " -> ";
             cost += data.getDistance(i, i + 1);
         }
         cost += data.getDistance(n, 1);
         cout << n << " -> " << 1 << endl;
         cout << "Custo de S: " << cost << endl;  */

    return 0;
}