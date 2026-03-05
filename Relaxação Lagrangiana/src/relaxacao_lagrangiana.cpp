#include <iostream>
#include <vector>
#include "Kruskal.h"
#include "Data.h"
#include "relaxacao_lagrangiana.h"
#include <limits>
#include <iomanip>
#include "relaxacao_funcoes.h"

using namespace std;

Tree relaxacao(Data &data)
{
    Tree best_tree = Subgradient(data, 148);
    return best_tree;
}

int main(int argc, char **argv)
{
    auto data = Data(argc, argv[1]);
    data.read();

    Tree best_tree = relaxacao(data);

    cout << "Tree final: " << endl;

    /*     for (int i = 0; i < data.getDimension(); i++)
        {
            cout << i << " | ";
            for (int &a : best_tree.listAdj[i])
            {
                cout << a << " - ";
            }
            cout << endl;
        } */

    cout << "Custo: " << best_tree.cost << endl;
}