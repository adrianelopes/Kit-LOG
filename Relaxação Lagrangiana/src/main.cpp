#include <iostream>
#include <vector>
#include "Kruskal.h"
#include "Data.h"
#include <limits>

using namespace std;

struct Tree
{
    double cost;
    vector<vector<int>> listAdj;
};

vector<vector<double>> modifica_Matriz(Data *data)
{
    int dim = data->getDimension() - 1;
    vector<vector<double>> matriz(dim, vector<double>(dim));
    for (int i = 2; i <= data->getDimension(); i++)
    {
        for (int j = 2; j <= data->getDimension(); j++)
        {
            matriz[i - 2][j - 2] = data->getDistance(i, j);
        }
    }
    return matriz;
}

Tree Make1_Tree_Original(Data &data)
{
    vector<vector<double>> matriz = modifica_Matriz(&data);
    auto kruskal = Kruskal(matriz);
    double inicost = kruskal.MST(data.getDimension() - 1);
    Tree tree;
    tree.listAdj = kruskal.getEdges();

    double cost;
    double second = numeric_limits<double>::infinity();
    double first = numeric_limits<double>::infinity();
    int firstJ = 0;
    int secondJ = 0;

    for (int j = 2; j < data.getDimension(); j++)
    {

        cost = data.getDistance(1, j);

        if (cost < first)
        {

            second = first;
            secondJ = firstJ;
            first = cost;
            firstJ = j - 1;
        }
        else if (cost < second)
        {
            second = cost;
            secondJ = j - 1;
        }
    }

    tree.cost = inicost + first + second;
    vector<int> J = {firstJ, secondJ};
    tree.listAdj.insert(tree.listAdj.begin(), J);
    tree.listAdj[firstJ].push_back(0);
    tree.listAdj[secondJ].push_back(0);

    return tree;
}

vector<vector<double>> altera_matriz(Data data, vector<double> lambda, vector<vector<double>> &matriz)
{

    for (int i = 1; i < data.getDimension(); i++)
    {
        for (int j = 1; j < data.getDimension(); j++)
        {
            cout << "I: " << i << " J: " << j << endl;
            matriz[i - 1][j - 1] = data.getDistance(i + 1, j + 1) - lambda[i] - lambda[j];
        }
    }

    cout << "Saindo de alterar matriz" << endl;
    return matriz;
}

Tree Make1_Tree(Data &data, vector<double> lambda)
{
    // Elimina a primeira linha e primeira coluna
    vector<vector<double>> matriz = modifica_Matriz(&data);
    cout << "Antes de alterar a matriz" << endl;
    matriz = altera_matriz(data, lambda, matriz);
    cout << "Depois de alterar a matriz" << endl;

    auto kruskal = Kruskal(matriz);
    double inicost = kruskal.MST(data.getDimension() - 1);
    Tree tree;
    tree.listAdj = kruskal.getEdges();

    double cost;
    double second = numeric_limits<double>::infinity();
    double first = numeric_limits<double>::infinity();
    int firstJ = 0;
    int secondJ = 0;

    /*     for (int i = 0; i < data.getDimension(); i++)
        {
            for (int j = 0; j < data.getDimension(); j++)
            {
                cout << data.getDistance(i + 1, j + 1) << " - ";
            }
            cout << endl;
        } */

    cout << "Antes de Criar a matriz" << endl;

    vector<double> matriz2(data.getDimension());

    cout << "Depois de Criar a matriz" << endl;

    for (int i = 0; i < data.getDimension(); i++)
    {
        cout << "Tamanho: " << matriz2.size() << endl;
        // erro aqui no getDistance
        cout << "vALOR: " << data.getDistance(2, 2) << endl;
        matriz2[i] = data.getDistance(1, i + 1) - lambda[i];
    }

    for (int j = 1; j < data.getDimension(); j++)
    {

        cost = matriz2[j];

        if (cost < first)
        {

            second = first;
            secondJ = firstJ;
            first = cost;
            firstJ = j;
        }
        else if (cost < second)
        {
            second = cost;
            secondJ = j;
        }
    }

    tree.cost = inicost + first + second;
    vector<int> J = {firstJ, secondJ};
    tree.listAdj.insert(tree.listAdj.begin(), J);
    tree.listAdj[firstJ].push_back(0);
    tree.listAdj[secondJ].push_back(0);

    return tree;
}

Tree Subgradient(Data &data, double UB)
{
    // Variáveis
    vector<double> lambda(data.getDimension()), bestlambda(data.getDimension());
    long double eps = 1, epsmin = 10;
    double wbest = 0, w = 0, mi = 0, sum = 0;
    int k = 0, kmax = 30, degree = 0;
    bool condition = true;
    Tree best_tree, tree, Original_tree;
    int count = 0;

    // Inicializando o vetor de penalizações com 0
    for (int i = 0; i < data.getDimension(); i++)
    {
        lambda[i] = 0;
    }

    // Gerando a árvore
    Original_tree = Make1_Tree_Original(data);

    tree = Original_tree;

    cout << "Tree criada " << endl;

    for (int i = 0; i < data.getDimension(); i++)
    {
        cout << i << " | ";
        for (int &a : Original_tree.listAdj[i])
        {
            cout << a << " - ";
        }
        cout << endl;
    }

    // eps < epsmin & condition == true
    while (count != 3)
    {
        cout << "----------------------------------------------" << endl;
        for (int i = 0; i < data.getDimension(); i++)
        {
            cout << "Lambda: " << i << "valor: " << lambda[i] << endl;
        }

        condition = false;

        w = tree.cost;
        if (w > wbest)
        {
            wbest = w;
            bestlambda = lambda;
            best_tree = tree;
            k = 0;
        }
        else
        {
            k = k + 1;
            if (k >= kmax)
            {
                k = 0;
                eps = eps / 2;
            }
        }

        for (int i = 1; i < data.getDimension(); i++)
        {

            degree = tree.listAdj[i].size();
            if (degree != 2)
            {
                condition = true;
            }
            sum = sum + ((2 - degree) * (2 - degree));
        }

        mi = eps * ((UB - w) / sum);

        for (int i = 1; i < data.getDimension(); i++)
        {
            degree = Original_tree.listAdj[i].size();
            lambda[i] = lambda[i] + (mi * (2 - degree));
        }
        cout << "Oi" << endl;
        cout << endl;
        tree = Make1_Tree(data, lambda);
        cout << "Tchau" << endl;
        cout << endl;
        count++;
    }

    return best_tree;
}

int main(int argc, char **argv)
{
    auto data = Data(argc, argv[1]);
    data.read();

    Tree best_tree = Subgradient(data, 148);

    cout << "Tree final: " << endl;

    for (int i = 0; i < data.getDimension(); i++)
    {
        cout << i << " | ";
        for (int &a : best_tree.listAdj[i])
        {
            cout << a << " - ";
        }
        cout << endl;
    }

    cout << "Custo: " << best_tree.cost << endl;
}