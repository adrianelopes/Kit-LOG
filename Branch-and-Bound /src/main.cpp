#include "Data.h"
#include "hungarian.h"
#include <iostream>
#include <vector>
#include <set>
using namespace std;

struct Node
{
  vector<pair<int, int>> forbidden_arcs;
  vector<int> smallersubtour;
  double lower_bound; // custo total da solucao do algoritmo hungaro
  bool feasible;      // indica se a solucao do AP_TSP e viavel
};

vector<int> Subtour(hungarian_problem_t &p)
{
  vector<int> smtour; // Menor subtour
  set<int> naoVisitados;
  int noAtual;
  int noStart;

  for (int i = 0; i < (p.num_rows); i++)
  {
    naoVisitados.insert(i);
    smtour.push_back(i);
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
    cout << "Subtour: " << endl;
    for (int i = 0; i < subtour.size(); i++)
    {
      cout << subtour[i] << " -> ";
    }
    cout << endl;
    if (subtour.size() <= smtour.size())
    {
      smtour = subtour;
    }
  }

  return smtour;
}

void updateNode(Node *node, Data *data, double **cost)
{
  for (int i = 0; i < data->getDimension(); i++)
  {
    for (int j = 0; j < data->getDimension(); j++)
    {
      if (i == j)
      {
        cost[i][j] = 9999999;
      }
      else
      {
        cost[i][j] = data->getDistance(i + 1, j + 1);
      }
    }
  }

  // for (auto aresta : node->forbidden_arcs){
  //     cost[aresta.first][aresta.second] = 999999;
  // }
  hungarian_problem_t p;
  int mode = HUNGARIAN_MODE_MINIMIZE_COST;
  hungarian_init(&p, cost, data->getDimension(), data->getDimension(),
                 mode); // Carregando o problema
  node->lower_bound = hungarian_solve(&p);
  node->smallersubtour = Subtour(p);                                        // detectar o conjunto de subtours
  node->feasible = node->smallersubtour.size() == data->getDimension() + 1; // verificar viabilidade
  hungarian_print_assignment(&p);
  hungarian_free(&p);
}

void printNo(Node *no)
{

  cout << "Menor subtour: " << endl;
  for (int i = 0; i < no->smallersubtour.size(); i++)
  {
    cout << no->smallersubtour[i] << " -> ";
  }
  cout << endl;

  cout << "Lower-Bound: " << no->lower_bound << endl;
}

void branch_and_bound(Data *data)
{
  double **cost = new double *[data->getDimension()];
  for (int i = 0; i < data->getDimension(); i++)
  {
    cost[i] = new double[data->getDimension()];
    for (int j = 0; j < data->getDimension(); j++)
    {
      if (i == j)
      {
        cost[i][j] = 9999999;
      }
      else
      {
        cost[i][j] = data->getDistance(i + 1, j + 1);
      }
    }
  }
  Node no;
  updateNode(&no, data, cost);
  printNo(&no);

  for (int i = 0; i < data->getDimension(); i++)
    delete[] cost[i];
  delete[] cost;
}

int main(int argc, char **argv)
{

  Data *data = new Data(argc, argv[1]);
  data->read();

  branch_and_bound(data);

  delete data;

  return 0;
}
