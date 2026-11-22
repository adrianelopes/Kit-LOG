#include <vector>

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

#define EPSILON 0.00000001

vector<vector<int>> MaxBack(double **x, int n) {
  vector<int> S0;
  vector<bool> vS(n, false);
  S0.push_back(0);
  vS[0] = true;
  double mincut_val = 0;
  double cut_val = 0;

  vector<double> maxback_val(n, 0);

  for (int j = 1; j < n; j++) {
    maxback_val[j] = x[min(0, j)][max(0, j)];
  }

  for (int j = 1; j < n; j++) {
    cut_val += maxback_val[j];
  }
  mincut_val = cut_val;

  double maximal = -1;
  int i = 0;

  vector<int> Sk = S0;
  vector<int> S;
  for (int k = 1; k < n; k++) {
    for (int j = 1; j < n; j++) {
      if (vS[j] == false) {
        if (maxback_val[j] > maximal) {
          maximal = maxback_val[j];
          i = j;
        }
      }
    }
    maximal = -1;
    vS[i] = true;
    Sk.push_back(i);
    cut_val = cut_val + (2 - 2 * maxback_val[i]);
    for (int j = 1; j < n; j++) {
      if (vS[j] == false) {
        maxback_val[j] += x[min(i, j)][max(i, j)];
      }
    }
    if (cut_val < mincut_val && Sk.size() != n) {
      mincut_val = cut_val;
      S = Sk;
    }
  }

  if (mincut_val < 2 - EPSILON) {
    return {S};
  }

  return {};
}

void imprime(vector<vector<int>> resultado) {
  if (resultado.empty()) {
    cout << "nenhum corte violado" << endl;
    return;
  }
  for (int c = 0; c < resultado.size(); c++) {
    cout << "S = { ";
    for (int v : resultado[c])
      cout << v << " ";
    cout << "}" << endl;
  }
}

int main() {
  int n = 6;

  double **x = new double *[n];
  for (int i = 0; i < n; i++) {
    x[i] = new double[n];
    for (int j = 0; j < n; j++)
      x[i][j] = 0;
  }

  // 0-1-2 e 3-4-5
  x[0][1] = 1;
  x[1][2] = 1;
  x[0][2] = 1;
  x[3][4] = 1;
  x[4][5] = 1;
  x[3][5] = 1;

  cout << "Teste 1 : ";
  imprime(MaxBack(x, n));

  // 0-1-2-3-4-5-0
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      x[i][j] = 0;

  x[0][1] = 1;
  x[1][2] = 1;
  x[2][3] = 1;
  x[3][4] = 1;
  x[4][5] = 1;
  x[0][5] = 1;

  cout << "Teste 2 : ";
  imprime(MaxBack(x, n));

  return 0;
}