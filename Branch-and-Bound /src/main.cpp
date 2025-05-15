#include "Data.h"
#include "hungarian.h"
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <queue>

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
  // hungarian_print_costmatrix(&p);
  node->lower_bound = hungarian_solve(&p);
  node->smallersubtour = Subtour(p);                                        // detectar o conjunto de subtours
  node->feasible = node->smallersubtour.size() == data->getDimension() + 1; // verificar viabilidade
  // hungarian_print_assignment(&p);

  hungarian_free(&p);
}

void printNo(Node *no, int tamanhoArvore, int noAtual, double upper_bound)
{

  int nosRestantes = tamanhoArvore - noAtual;

  cout << noAtual << " " << nosRestantes << " " << tamanhoArvore << " " << no->lower_bound << " " << no->feasible << " " << upper_bound << endl;

  // cout << "Menor subtour: " << endl;

  /* for (int i = 0; i < no->smallersubtour.size(); i++)
  {
    cout << no->smallersubtour[i] << " -> ";
  }
  cout << endl;

  for (auto aresta : no->forbidden_arcs)
  {
    cout << "Arco: " << aresta.first << " -> " << aresta.second << endl;
  } */
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
  int contadornos = 0;
  int quant = 0;
  int arvore = 0;

  Node root;
  updateNode(&root, data, cost);

  if (tipo == 3)
  {
    priority_queue<Node> tree;
    tree.push(root);
    arvore++;
    contadornos++;
    quant++;

    while (!tree.empty())
    {
      Node node = tree.top();
      tree.pop();
      contadornos++;
      quant++;
      arvore--;

      // Cortar os nós com valores piores que o upper_bound
      if (node.lower_bound > upper_bound)
      {
        continue;
      }

      if (node.feasible)
      {
        upper_bound = min(upper_bound, node.lower_bound);
      }
      else
      {
        for (int i = 0; i < node.smallersubtour.size() - 1; i++)
        {
          Node son;
          contadornos++;
          quant++;
          son.forbidden_arcs = node.forbidden_arcs;
          pair<int, int> forbidden_arc = {
              node.smallersubtour[i],
              node.smallersubtour[i + 1]};
          son.forbidden_arcs.push_back(forbidden_arc);
          updateNode(&son, data, cost);
          if (contadornos >= 100)
          {
            printNo(&son, tree.size(), quant, upper_bound);
            contadornos = 0;
          }
          if (son.lower_bound < upper_bound)
          {
            tree.push(son);
          }
        }
      }
      // cout << "Tamamho da arvore: " << tree.size() << '\n';
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

      // Cortar os nós com valores piores que o upper_bound
      if (node.lower_bound > upper_bound)
      {
        continue;
      }

      if (node.feasible)
      {
        upper_bound = min(upper_bound, node.lower_bound);
      }
      else
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
          /* cout << "Pai: " << endl;
          printNo(&node);
          cout << "Filho: " << endl;
          printNo(&son); */
          if (son.lower_bound < upper_bound)
          {
            tree.push_back(son);
          }
        }
      }
      // cout << "Tamamho da arvore: " << tree.size() << '\n';
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

  cout << "NoAtual" << " " << "NosRestantes" << " " << "TamanhoArvore" << " " << "LowerBound" << " " << "Viabilidade" << " " << "UpperBound" << endl;

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
