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
    // cout << "DIM: " << dim << endl;
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

Tree Make1_Tree(Data &data, vector<double> lambda)
{
    vector<vector<double>> matriz = modifica_Matriz(&data);
    auto kruskal = Kruskal(matriz);
    double inicost = kruskal.MST(data.getDimension() - 1);
    Tree tree;
    tree.listAdj = kruskal.getEdges();

    cout << "Lambda dentro da make_tree" << endl;
    for (int i = 0; i < data.getDimension(); i++)
    {
        cout << "Lambda: " << i << "valor: " << lambda[i] << endl;
    }

    cout << "Antes: " << endl;
    cout << "[2][1]: " << data.distMatrix[2][1] << endl;

    // Alterar  matriz distância
    for (int l = 1; l < lambda.size(); l++)
    {
        for (int j = 0; j < data.getDimension(); j++)
        {
            for (int i = 0; i < data.getDimension(); i++)
            {
                data.distMatrix[i][j] = data.distMatrix[i][j] - lambda[l];
                // cout << "I: " << i << " J: " << j << " Valor: " << data.distMatrix[i][j] << endl;
            }
        }
    }
    cout << "Depois: " << endl;

    cout << "[2][1]: " << data.distMatrix[2][1] << endl;

    // cout << "Tamanho listAdj: " << tree.listAdj.size() << endl;

    double cost;
    double second = numeric_limits<double>::infinity();
    double first = numeric_limits<double>::infinity();
    int firstJ = 0;
    int secondJ;

    // cout << "firstJ: " << firstJ << ", secondJ: " << secondJ << endl;

    for (int j = 2; j < data.getDimension(); j++)
    {

        cost = data.getDistance(1, j);
        // cout << "Cost: " << cost << endl;

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

    // cout << "firstJ: " << firstJ << ", secondJ: " << secondJ << endl;

    tree.cost = inicost + first + second;
    vector<int> J = {firstJ, secondJ};
    tree.listAdj.insert(tree.listAdj.begin(), J);
    tree.listAdj[firstJ].push_back(0);
    tree.listAdj[secondJ].push_back(0);

    return tree;
}

Tree Subgradient(Data &data, double UB)
{
    vector<double> lambda(data.getDimension()), bestlambda(data.getDimension());
    long double eps = 1, epsmin = 10;
    double wbest = 0, w = 0, mi = 0, sum = 0;
    int k = 0, kmax = 30, degree = 0;
    bool condition = true;
    Tree best_tree, tree;
    int count = 0;
    for (int i = 0; i < data.getDimension(); i++)
    {
        lambda[i] = 0;
    }

    // eps < epsmin & condition == true
    while (count != 2)
    {
        /*   cout << "----------------------------------------------" << endl;
          for (int i = 0; i < data.getDimension(); i++)
          {
              cout << "Lambda: " << i << "valor: " << lambda[i] << endl;
          } */

        tree = Make1_Tree(data, lambda);
        condition = false;

        /*  cout << "Tree criada " << endl;

         for (int i = 0; i < data.getDimension(); i++)
         {
             cout << i << " | ";
             for (int &a : tree.listAdj[i])
             {
                 cout << a << " - ";
             }
             cout << endl;
         } */
        cout << "Custo da tree atual: " << tree.cost << endl;
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
            degree = tree.listAdj[i].size();
            lambda[i] = lambda[i] + (mi * (2 - degree));
        }
        count++;
    }

    return best_tree;
}

int main(int argc, char **argv)
{
    auto data = Data(argc, argv[1]);
    data.read();

    Tree best_tree = Subgradient(data, 500000);
    /*
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

        cout << "Custo: " << best_tree.cost << endl; */
}