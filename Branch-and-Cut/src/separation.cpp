#include "separation.h"
#include <vector>

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

vector<vector<int>> MinCut(double **x, int n) { return {}; }