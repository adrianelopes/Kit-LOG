#include "Data.h"
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

struct Node
{
    vector<pair<int, int>> forbidden_arcs;
    vector<int> smallersubtour;
    double lower_bound; // custo total da solucao do algoritmo hungaro
    bool feasible;      // indica se a solucao do AP_TSP é viavel

    bool operator<(const Node &next) const
    {
        return lower_bound > next.lower_bound;
    }
};

// Trocar o húngaro pela relaxação

/* vector<int> Subtour(hungarian_problem_t &p)
{
  vector<int> smtour; // Menor subtour
  set<int> naoVisitados;
  int noAtual;
  int noStart;

  for (int i = 0; i < (p.num_rows); i++)
  {
    naoVisitados.insert(i);
  }

  while (!naoVisitados.empty())
  {
    vector<int> subtour;
    noStart = *naoVisitados.begin();
    noAtual = noStart;
    subtour.push_back(noStart);
    naoVisitados.erase(noAtual);

    for (int j = 0; j < p.num_cols; j++)
    {
      if (p.assignment[noAtual][j] == 1)
      {
        if (j == noStart)
        {
          subtour.push_back(noStart);
          break;
        }
        else
        {
          subtour.push_back(j);
          noAtual = j;
          naoVisitados.erase(noAtual);
          j = -1;
        }
      }
    }

    if ((subtour.size() < smtour.size()) || smtour.empty())
    {
      smtour = subtour;
    }
  }

  return smtour;
} */

// Criar função que:
// Usa o dual lagrangiano
// Verifica se a melhor árvore obtida é um tour
// Se não for: encontra o vértice com maior grau, proibe os arcos associados a esse vértice nos nós filhos

int main(int argc, char **argv)
{

    Data *data = new Data(argc, argv[1]);
    data->read();

    int tipo = stoi(argv[2]);

    std::cout << std::setw(8) << "Node"
              << std::setw(8) << "Left"
              << std::setw(12) << "Tree"
              << std::setw(14) << "BestInt"
              << std::setw(14) << "BestBd"
              << std::setw(10) << "ItrCnt"
              << std::setw(10) << "Gap(%)"
              << "\n";

    if (argc == 4)
    {
        cout << branch_and_bound(data, stod(argv[3]) + 1, tipo) << endl;
    }
    else
    {
        cout << branch_and_bound(data, 99999999, tipo) << endl;
    }

    delete data;

    return 0;
}
