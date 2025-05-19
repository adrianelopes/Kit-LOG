#include "Data.h"
#include "hungarian.h"
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
  bool feasible;      // indica se a solucao do AP_TSP e viavel

  bool operator<(const Node &next) const
  {
    return lower_bound > next.lower_bound;
  }
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
    /* cout << "Subtour: " << endl;
    for (int i = 0; i < subtour.size(); i++)
    {
    cout << subtour[i] << " -> ";
    }
    cout << endl; */
    if ((subtour.size() < smtour.size()) || smtour.empty())
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

  for (auto aresta : node->forbidden_arcs)
  {
    cost[aresta.first][aresta.second] = 9999999;
  }
  hungarian_problem_t p;
  int mode = HUNGARIAN_MODE_MINIMIZE_COST;
  hungarian_init(&p, cost, data->getDimension(), data->getDimension(),
                 mode); // Carregando o problema
  node->lower_bound = hungarian_solve(&p);
  node->smallersubtour = Subtour(p);                                        // detectar o conjunto de subtours
  node->feasible = node->smallersubtour.size() == data->getDimension() + 1; // verificar viabilidade

  hungarian_free(&p);
}

void printNo(long long nodeCount, long long left, long long treeSz, double bestInteger, double bestBound, long long iterCount, double gap)
{

  cout << setw(8) << nodeCount
       << setw(8) << left
       << setw(12) << treeSz
       << setw(14) << fixed << setprecision(2) << bestInteger
       << setw(14) << fixed << setprecision(2) << bestBound
       << setw(10) << iterCount
       << setw(10) << fixed << setprecision(2) << gap
       << "\n";
}

double branch_and_bound(Data *data, double upper_bound, int tipo)
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

  Node root;
  updateNode(&root, data, cost);
  long long nodeCount = 0;             // Nós processados
  long long iterCount = 0;             // Contador de cortes/interações
  double bestInteger = upper_bound;    // Melhor solução inteira até agora
  double bestBound = root.lower_bound; // Melhor limite inferior

  if (tipo == 3)
  {
    priority_queue<Node> tree;
    tree.push(root);

    while (!tree.empty())
    {
      Node node = tree.top();
      tree.pop();
      nodeCount++;
      iterCount++;

      // Cortar os nós com valores piores que o upper_bound
      if (node.lower_bound > upper_bound)
      {
        continue;
      }

      if (node.feasible)
      {
        upper_bound = min(upper_bound, node.lower_bound);
        // atualiza melhor inteiro
        if (node.lower_bound < bestInteger)
        {
          bestInteger = node.lower_bound;
        }
      }

      // atualiza melhor bound
      bestBound = (!tree.empty() ? tree.top().lower_bound : node.lower_bound);

      if (nodeCount % 100 == 0)
      {
        long long left = (long long)tree.size();
        long long treeSz = (long long)tree.size();
        double gap = (bestInteger > 0.0) ? 100.0 * (bestInteger - bestBound) / bestInteger : 0.0;
        printNo(nodeCount, left, treeSz, bestInteger, bestBound, iterCount, gap);
      }

      if (bestInteger == bestBound)
      {
        long long left = (long long)tree.size();
        long long treeSz = (long long)tree.size();
        double gap = (bestInteger > 0.0) ? 100.0 * (bestInteger - bestBound) / bestInteger : 0.0;
        printNo(nodeCount, left, treeSz, bestInteger, bestBound, iterCount, gap);

        break;
      }

      if (!node.feasible)
      {
        for (int i = 0; i < node.smallersubtour.size() - 1; i++)
        {
          Node son;

          son.forbidden_arcs = node.forbidden_arcs;
          pair<int, int> forbidden_arc = {
              node.smallersubtour[i],
              node.smallersubtour[i + 1]};
          son.forbidden_arcs.push_back(forbidden_arc);
          updateNode(&son, data, cost);
          if (son.lower_bound < upper_bound)
          {
            tree.push(son);
          }
        }
      }
    }
  }

  else
  {
    deque<Node> tree;
    tree.push_back(root);
    Node node;

    while (!tree.empty())
    {
      if (tipo == 1)
      {
        node = tree.back();
        tree.pop_back();
      }
      else
      {
        node = tree.front();
        tree.pop_front();
      }
      nodeCount++;
      iterCount++;

      // Cortar os nós com valores piores que o upper_bound
      if (node.lower_bound > upper_bound)
      {
        continue;
      }

      if (node.feasible)
      {
        upper_bound = min(upper_bound, node.lower_bound);
        if (node.lower_bound < bestInteger)
        {
          bestInteger = node.lower_bound;
        }
      }

      // atualiza melhor bound
      if (!tree.empty())
      {
        double bestLb = tree.front().lower_bound;
        for (const auto &n : tree)
          bestLb = min(bestLb, n.lower_bound);
        bestBound = bestLb;
      }
      if (nodeCount % 100 == 0)
      {
        long long left = (long long)tree.size();
        long long treeSz = (long long)tree.size();
        double gap = (bestInteger > 0.0) ? 100.0 * (bestInteger - bestBound) / bestInteger : 0.0;
        printNo(nodeCount, left, treeSz, bestInteger, bestBound, iterCount, gap);
      }

      if (bestInteger == bestBound)
      {
        long long left = (long long)tree.size();
        long long treeSz = (long long)tree.size();
        double gap = (bestInteger > 0.0) ? 100.0 * (bestInteger - bestBound) / bestInteger : 0.0;
        printNo(nodeCount, left, treeSz, bestInteger, bestBound, iterCount, gap);
        break;
      }

      if (!node.feasible)
      {
        for (int i = 0; i < node.smallersubtour.size() - 1; i++)
        {
          Node son;
          son.forbidden_arcs = node.forbidden_arcs;
          pair<int, int> forbidden_arc = {
              node.smallersubtour[i],
              node.smallersubtour[i + 1]};
          son.forbidden_arcs.push_back(forbidden_arc);
          updateNode(&son, data, cost);
          if (son.lower_bound < upper_bound)
          {
            tree.push_back(son);
          }
        }
      }
    }
  }

  for (int i = 0; i < data->getDimension(); i++)
    delete[] cost[i];
  delete[] cost;
  return upper_bound;
}

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
