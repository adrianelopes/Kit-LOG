#include <iostream>
#include <vector>
#include "Kruskal.h"
#include "Data.h"

struct Tree
{
    double cost;
    vector<vector<int>> listAdj;
};

vector<vector<double>> modifica_Matriz(Data *data)
{
    vector<vector<double>> matriz;
    for (int i = 1; i < data->getDimension(); i++)
    {
        for (int j = 1; j < data->getDimension(); j++)
        {
            matriz[i - 1][j - 1] = data->getDistance(i, j);
        }
    }
    return matriz;
}

Tree Make1_Tree(Data &data)
{
    vector<vector<double>> matriz = modifica_Matriz(&data);
    auto kruskal = Kruskal(matriz);
    Tree tree;
    tree.listAdj = kruskal.getEdges();
    double inicost = kruskal.MST(data.getDimension() - 1);
    double cost;
    double second = 0;
    double first;
    int secondJ;
    int firstJ = 1;

    for (int j = 1; j < data.getDimension() - 1; j++)
    {
        cost = data.getDistance(0, j);

        if ((cost < first) || j == 1)
        {
            second = first;
            secondJ = firstJ;
            first = cost;
            firstJ = j;
        }
        else if (cost < second)
        {
            second = cost;
        }
    }

    tree.cost = inicost + first + second;
    vector<int> J = {firstJ, secondJ};
    tree.listAdj[firstJ].push_back(0);
    tree.listAdj[secondJ].push_back(0);
    tree.listAdj.insert(tree.listAdj.begin(), J);
    return tree;
}

int main(int argc, char **argv)
{
    auto data = Data(argc, argv[1]);
    Tree tree = Make1_Tree(data);
    for (int i = 0; i < data.getDimension(); i++)
    {
        cout << i << " | ";
        for (int &a : tree.listAdj[i])
        {
            cout << a << " - ";
        }
    }
}