#include "Data.h"
#include "hungarian.h"
#include <iostream>
#include <vector>
using namespace std;

struct Node {
  vector<pair<int, int>> forbidden_arcs;
  vector<vector<int>> subtour;
  double lower_bound; // custo total da solucao do algoritmo hungaro
  int chosen;         // indice do menor subtour
  bool feasible;      // indica se a solucao do AP_TSP e viavel
};

struct Node Subtours(hungarian_problem_t *p) {}

void updateNode(Node *node, Data *data, double **cost, hungarian_problem_t p) {
  int mode = HUNGARIAN_MODE_MINIMIZE_COST;
  hungarian_init(&p, cost, data->getDimension(), data->getDimension(),
                 mode); // Carregando o problema
  node->lower_bound = hungarian_solve(&p);

  /* node->subtour = Subtours(p); // detectar o conjunto de subtours

  node->chosen = getChoosen(p); // pegar o indice do menor subtour

  node->feasible = isFeasible(p); // verificar viabilidade */

  hungarian_free(&p);
}

void branch_and_bound() {}

int main(int argc, char **argv) {

  Data *data = new Data(argc, argv[1]);
  data->read();

  double **cost = new double *[data->getDimension()];
  for (int i = 0; i < data->getDimension(); i++) {
    cost[i] = new double[data->getDimension()];
    for (int j = 0; j < data->getDimension(); j++) {
      cost[i][j] = data->getDistance(i, j);
    }
  }
  hungarian_problem_t p;
  double obj_value = hungarian_solve(&p);
  cout << "Obj. value: " << obj_value << endl;

  cout << "Assignment" << endl;
  hungarian_print_assignment(&p);

  hungarian_free(&p);
  for (int i = 0; i < data->getDimension(); i++)
    delete[] cost[i];
  delete[] cost;
  delete data;

  return 0;
}
