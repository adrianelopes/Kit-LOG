
#include <iostream>
#include <vector>
#include "relaxacao_funcoes.h"
#include <limits>
#include <iomanip>

using namespace std;

vector<vector<double>> modifica_Matriz(Data *data, const vector<pair<int, int>> &forbidden_arcs)
{
    int dim = data->getDimension() - 1;

    vector<vector<double>> matriz(dim, vector<double>(dim));

    // cria matriz sem o nó 0
    for (int i = 2; i <= data->getDimension(); i++)
    {
        for (int j = 2; j <= data->getDimension(); j++)
        {
            matriz[i - 2][j - 2] = data->getDistance(i, j);
        }
    }

    double INF = numeric_limits<double>::infinity();

    // aplica arcos proibidos
    for (auto &arc : forbidden_arcs)
    {
        int u = arc.first;
        int v = arc.second;

        if (u == 1 || v == 1)
        {
            continue;
        }

        int i = u - 1;
        int j = v - 1;

        if (i >= 0 && i < dim && j >= 0 && j < dim)
        {
            matriz[i][j] = INF;
            matriz[j][i] = INF;
        }
    }

    return matriz;
}

// Alterar a matriz de acordo com a solução
void altera_matriz(Data *data, double **cost, vector<double> &lambda, vector<vector<double>> &matriz)
{

    for (int i = 1; i < data->getDimension(); i++)
    {
        for (int j = 1; j < data->getDimension(); j++)
        {
            matriz[i - 1][j - 1] = matriz[i - 1][j - 1] - lambda[i] - lambda[j];
        }
    }
}

// Criar a árvore de acordo com a matriz alterada
Tree Make1_Tree(Data *data, double **cost, vector<double> &lambda, const vector<pair<int, int>> &forbidden_arcs)
{

    // cout << "antes do modifica matriz" << endl;
    vector<vector<double>> matriz = modifica_Matriz(data, forbidden_arcs);
    // cout << "antes do altera matriz" << endl;

    altera_matriz(data, cost, lambda, matriz);
    // cout << "depois do altera matriz" << endl;

    auto kruskal = Kruskal(matriz);
    double inicost = kruskal.MST(data->getDimension() - 1);
    Tree tree;
    tree.listAdj = kruskal.getEdges();

    /*     cout << "Graus da 1-tree:\n";

        for (int i = 0; i < tree.listAdj.size(); i++)
        {
            cout << i << " -> " << tree.listAdj[i].size() << endl;
        } */

    double custo;
    double second = numeric_limits<double>::infinity();
    double first = numeric_limits<double>::infinity();
    int firstJ = 0;
    int secondJ = 0;

    vector<double> matriz2(data->getDimension());

    // Matriz das distâncias do vétice 0 para os outros vértices
    for (int i = 0; i < data->getDimension(); i++)
    {
        matriz2[i] = cost[0][i] - lambda[i];
    }

    for (int j = 1; j < data->getDimension(); j++)
    {
        bool forbidden = false;

        for (auto &arc : forbidden_arcs)
        {

            if ((arc.first == 0 && arc.second == j) ||
                (arc.second == 0 && arc.first == j) ||
                (arc.first == 1 && arc.second == j + 1) ||
                (arc.second == 1 && arc.first == j + 1))
            {
                forbidden = true;
                break;
            }
        }

        if (forbidden)
        {
            continue;
        }

        custo = matriz2[j];

        if (custo < first)
        {

            second = first;
            secondJ = firstJ;
            first = custo;
            firstJ = j;
        }
        else if (custo < second)
        {
            second = custo;
            secondJ = j;
        }
    }

    double sum_lambda = 0.0;
    for (int i = 1; i < data->getDimension(); ++i)
    {
        sum_lambda += lambda[i];
    }

    tree.cost = inicost + first + second + 2.0 * sum_lambda;
    vector<int> J = {firstJ, secondJ};
    tree.listAdj.insert(tree.listAdj.begin(), J);
    tree.listAdj[firstJ].push_back(0);
    tree.listAdj[secondJ].push_back(0);

    return tree;
}

Tree Subgradient(Data *data, double **cost, double UB, vector<double> &lambda, const vector<pair<int, int>> &forbidden_arcs)
{

    // Variáveis
    vector<double> bestlambda(data->getDimension());
    long double eps = 1, epsmin = 0.0001;
    double wbest = 0, w = 0, mi = 0, sum = 0;
    int k = 0, kmax = 30, degree = 0;
    bool condition = true;
    Tree best_tree, tree, Original_tree;
    int count = 0;

    while (eps > epsmin && condition)
    {

        condition = false;
        tree = Make1_Tree(data, cost, lambda, forbidden_arcs);
        tree.lambda = lambda;

        w = tree.cost;

        if (w > wbest)
        {

            wbest = w;
            bestlambda = lambda;
            best_tree = tree;
            best_tree.lambda = tree.lambda;
            k = 0;
        }
        else
        {

            k = k + 1;
            if (k >= kmax)
            {
                k = 0;
                eps = (eps / 2);
            }
        }

        sum = 0;
        // Verificar se a solução atual obedece a restrição de grau
        for (int i = 1; i < data->getDimension(); i++)
        {
            // Erro no cálculo do grau
            degree = tree.listAdj[i].size();
            if (degree != 2)
            {
                condition = true;
            }
            sum = sum + ((2 - degree) * (2 - degree));
        }

        if (sum == 0)
            break;

        mi = eps * ((UB - w) / sum);

        for (int i = 1; i < data->getDimension(); i++)
        {
            // Analisar se aqui não é tree ao invés de original tree
            degree = tree.listAdj[i].size();
            lambda[i] = lambda[i] + (mi * (2 - degree));
        }
        count++;
    }

    return best_tree;
}