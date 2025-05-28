#include <iostream>
#include <vector>
#include "Kruskal.h"
#include "Data.h"
#include <limits>

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
    double inicost = kruskal.MST(data.getDimension() - 1);
    Tree tree;
    tree.listAdj = kruskal.getEdges();

    std::cout << "Tamanho listAdj: " << tree.listAdj.size() << std::endl;

    double cost;
    double second = std::numeric_limits<double>::infinity();
    double first = std::numeric_limits<double>::infinity();
    int firstJ = -1; // Inicializa índice inválido para detectar erros
    int secondJ = -1;

    for (int j = 1; j < data.getDimension() - 1; j++)
    {
        cost = data.getDistance(0, j);

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

    std::cout << "firstJ: " << firstJ << ", secondJ: " << secondJ << std::endl;

    int n = data.getDimension();
    // Redimensionar listAdj para garantir espaço para o vértice 0
    if ((int)tree.listAdj.size() < n)
    {
        tree.listAdj.resize(n);
    }

    tree.cost = inicost + first + second;
    vector<int> J = {firstJ, secondJ};
    tree.listAdj.insert(tree.listAdj.begin(), J);
    firstJ++;
    secondJ++;

    tree.listAdj[firstJ].push_back(0);
    tree.listAdj[secondJ].push_back(0);

    return tree;
}

int main(int argc, char **argv)
{
    auto data = Data(argc, argv[1]);
    data.read();
    cout << "Dimensão: " << data.getDimension() << endl;
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