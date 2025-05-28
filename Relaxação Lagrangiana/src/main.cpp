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
    cout << "DIM: " << dim << endl;
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

Tree Make1_Tree(Data &data)
{
    vector<vector<double>> matriz = modifica_Matriz(&data);
    auto kruskal = Kruskal(matriz);
    double inicost = kruskal.MST(data.getDimension() - 1);
    Tree tree;
    tree.listAdj = kruskal.getEdges();

    cout << "Tamanho listAdj: " << tree.listAdj.size() << endl;

    double cost;
    double second = numeric_limits<double>::infinity();
    double first = numeric_limits<double>::infinity();
    int firstJ = 0;
    int secondJ;

    cout << "firstJ: " << firstJ << ", secondJ: " << secondJ << endl;

    for (int j = 2; j < data.getDimension(); j++)
    {

        cost = data.getDistance(1, j);
        cout << "Cost: " << cost << endl;

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

    cout << "firstJ: " << firstJ << ", secondJ: " << secondJ << endl;

    tree.cost = inicost + first + second;
    vector<int> J = {firstJ, secondJ};
    tree.listAdj.insert(tree.listAdj.begin(), J);
    tree.listAdj[firstJ].push_back(0);
    tree.listAdj[secondJ].push_back(0);

    return tree;
}

Tree Subgradient(Data &data)
{
    vector<double> λ;
    long double ε = 1;
    long double εmin = 10;
    double wbest = 0;
    while ()
    {
        Tree tree = Make1_Tree(data);
        for (int i = 0; i < data.getDimension(); i++)
        {
            cout << i << " | ";
            for (int &a : tree.listAdj[i])
            {
                cout << a << " - ";
            }
            cout << endl;
        }
        double w = tree.cost;
        if (w > wbest)
        {
            wbest = w;
        }
    }
}

int main(int argc, char **argv)
{
    auto data = Data(argc, argv[1]);
    data.read();
}