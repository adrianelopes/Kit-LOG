#include "Data.h"
#include "debug.h"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#define NDEBUG
#include <cassert>
#include <chrono>
using namespace chrono;
using namespace std;

struct Subsequence {
  double T, C;
  int W;
  int first, last;
  inline static Subsequence Concatenate(Subsequence &sigma_1,
                                        Subsequence &sigma_2, Data &data) {
    Subsequence sigma;

    double temp = data.getDistance(sigma_1.last, sigma_2.first);
    sigma.W = sigma_1.W + sigma_2.W;
    sigma.T = sigma_1.T + temp + sigma_2.T;
    sigma.C = sigma_1.C + sigma_2.W * (sigma_1.T + temp) + sigma_2.C;
    sigma.first = sigma_1.first;
    sigma.last = sigma_2.last;
    return sigma;
  }
};

void UpdateAllSubseq(Solution *s, vector<vector<Subsequence>> &subseq_matrix,
                     Data &data, int start, int finish) {
  int n = s->sequence.size();

  for (int i = 0; i < n; i++) {
    int v = s->sequence[i];
    subseq_matrix[i][i].W = (i > 0);
    subseq_matrix[i][i].C = 0;
    subseq_matrix[i][i].T = 0;
    subseq_matrix[i][i].first = s->sequence[i];
    subseq_matrix[i][i].last = s->sequence[i];
  }

  // Com i sendo linhas e j sendo as colunas
  // Primeiro as linhas variam de start até n e as colunas variam de 0 até
  // finish
  for (int i = start; i <= n; i++) {
    for (int j = 0; j < finish; j++) {
      subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j - 1],
                                                     subseq_matrix[j][j], data);
    }
  }

  // Agora as linhas variam de 0 até finish e as colunas variam de start até n
  for (int i = 1; i <= finish; i++) {
    for (int j = start; j <= n; j++) {
      subseq_matrix[i][j] = Subsequence::Concatenate(subseq_matrix[i][j + 1],
                                                     subseq_matrix[j][j], data);
    }
  }

  s->valorobj = subseq_matrix[0][n - 1].C;
}

struct InsertionInfo {
  int noInserido;
  double custo;
};

void exibirSolucao(Solution &s) {

  for (int i = 0; i < s.sequence.size() - 1; i++)
    std::cout << s.sequence[i] << " -> ";
  std::cout << s.sequence.back() << std::endl;
  // cout << "Custo da Solução: " << s.valorobj << endl;
}

vector<InsertionInfo> calcularCusto(Solution &s, vector<int> &CL, Data &data) {

  // número de combinações possíveis
  int n = CL.size();
  vector<InsertionInfo> custoInsercao(n);

  int l = 0;
  double aux = 0;
  double acumulado = s.custoAcumulado;
  auto position = 1;
  int i = 0;

  for (auto k : CL) {
    i = s.sequence.back();
    custoInsercao[l].noInserido = k;
    custoInsercao[l].custo = data.getDistance(i, k);
    l++;
  }

  return custoInsercao;
}

bool compararCusto(InsertionInfo &a, InsertionInfo &b) {
  return a.custo < b.custo;
}

Solution Construcao(Data &data) {
  Solution s;
  vector<int> CL;

  // inicializando CL
  int n = data.getDimension();
  for (int a = 2; a <= n; a++) {
    CL.push_back(a);
  }

  s.sequence.push_back(1);

  while (!CL.empty()) {
    vector<InsertionInfo> custoInsercao = calcularCusto(s, CL, data);
    sort(custoInsercao.begin(), custoInsercao.end(), compararCusto);
    double alpha = ((double)rand() / RAND_MAX) / 4;
    int selecionado = rand() % ((int)ceil(alpha * (custoInsercao.size())));
    int inserir = custoInsercao[selecionado].noInserido;
    s.sequence.push_back(inserir);
    CL.erase(remove(CL.begin(), CL.end(), inserir));
  }
  s.sequence.push_back(1);
  assert(verificaConstrucao(data, s));
  return s;
}

bool Swap(Solution &s, Data &data, vector<vector<Subsequence>> &subseq_matrix) {
  double bestDelta = 0;
  int best_i, best_j;
  double delta = 0;
  // int j, i;
  Subsequence sigma_1, sigma_2, sigma_3, sigma;
  int n = s.sequence.size() - 1;

  for (int i = 1; i < s.sequence.size() - 1; i++) {

    for (int j = i + 1; j < s.sequence.size() - 1; j++) {

      if (j == i + 1) {
        sigma_1 = Subsequence::Concatenate(subseq_matrix[0][i - 1],
                                           subseq_matrix[j][j], data);
        sigma_2 = Subsequence::Concatenate(sigma_1, subseq_matrix[i][i], data);
        sigma =
            Subsequence::Concatenate(sigma_2, subseq_matrix[j + 1][n], data);
      } else {
        sigma_1 = Subsequence::Concatenate(subseq_matrix[0][i - 1],
                                           subseq_matrix[j][j], data);
        sigma_2 = Subsequence::Concatenate(sigma_1, subseq_matrix[i + 1][j - 1],
                                           data);
        sigma_3 = Subsequence::Concatenate(sigma_2, subseq_matrix[i][i], data);
        sigma =
            Subsequence::Concatenate(sigma_3, subseq_matrix[j + 1][n], data);
      }

      // assert(verificaSwap(data, s, i, j, delta));
      double delta = sigma.C - s.valorobj;

      if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }

  if (bestDelta < 0) {

    swap(s.sequence[best_i], s.sequence[best_j]);
    UpdateAllSubseq(&s, subseq_matrix, data, best_i, best_j);

    return true;
  }

  return false;
}

bool twoOpt(Solution &s, Data &data,
            vector<vector<Subsequence>> &subseq_matrix) {
  double bestDelta = 0;
  int best_i, best_j, i, j;
  double delta = 0;
  Subsequence sigma_1, sigma;
  int n = s.sequence.size() - 1;

  for (i = 1; i < s.sequence.size() - 2; i++) {

    for (j = i + 1; j < s.sequence.size() - 1; j++) {
      sigma_1 = Subsequence::Concatenate(subseq_matrix[0][i - 1],
                                         subseq_matrix[j][i], data);
      sigma = Subsequence::Concatenate(sigma_1, subseq_matrix[j + 1][n], data);
      // assert(verificaTwoopt(data, s, i, j, delta));

      double delta = sigma.C - s.valorobj;

      if (delta < bestDelta) {

        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }

  if (bestDelta < 0) {
    reverse(s.sequence.begin() + best_i, s.sequence.begin() + best_j + 1);
    UpdateAllSubseq(&s, subseq_matrix, data, best_i, best_j);
    // assert(verificaValorDelta(data, s, s.valorobj));

    return true;
  }

  return false;
}

bool orOpt(Solution &s, Data &data, vector<vector<Subsequence>> &subseq_matrix,
           int n) {
  double bestDelta = 0;
  int best_i, best_j, i, j;
  double delta = 0;
  Subsequence sigma_1, sigma_2, sigma;
  int x = n - 1;
  bool verifica = false;
  int k = s.sequence.size() - 1;

  for (i = 1; i < s.sequence.size() - n; i++) {

    for (j = 0; j < s.sequence.size() - 1; j++) {
      for (int teste = i - 1; teste <= i + x + 1; teste++) {
        if (j == teste) {
          verifica = true;
          break;
        }
      }

      if (verifica == true) {
        verifica = false;
        continue;
      }

      if (i < j) {
        sigma_1 = Subsequence::Concatenate(subseq_matrix[0][i - 1],
                                           subseq_matrix[i + x + 1][j], data);
        sigma_2 =
            Subsequence::Concatenate(sigma_1, subseq_matrix[i][i + x], data);
        sigma =
            Subsequence::Concatenate(sigma_2, subseq_matrix[j + 1][k], data);
      }

      else if (i > j) {
        sigma_1 = Subsequence::Concatenate(subseq_matrix[0][j],
                                           subseq_matrix[i][i + x], data);
        sigma_2 = Subsequence::Concatenate(sigma_1, subseq_matrix[j + 1][i - 1],
                                           data);
        sigma = Subsequence::Concatenate(sigma_2, subseq_matrix[i + x + 1][k],
                                         data);
      }

      double delta = sigma.C - s.valorobj;
      assert(verificaOrOpt(data, s, i, j, n, delta));

      if (delta < bestDelta) {
        bestDelta = delta;
        best_i = i;
        best_j = j;
      }
    }
  }

  if (bestDelta < 0) {

    if (best_j < best_i) {
      rotate(s.sequence.begin() + best_j + 1, s.sequence.begin() + best_i,
             s.sequence.begin() + (best_i + n));
      UpdateAllSubseq(&s, subseq_matrix, data, best_i, best_j);

      return true;
    } else {
      rotate(s.sequence.begin() + best_i, s.sequence.begin() + best_i + n,
             s.sequence.begin() + best_j + 1);
      UpdateAllSubseq(&s, subseq_matrix, data, best_i, best_j);

      return true;
    }
  } else {
    return false;
  }
}

void buscaLocal(Solution &s, Data &data,
                vector<vector<Subsequence>> &subseq_matrix) {
  vector<int> NL = {1, 2, 3, 4, 5};
  bool improved = false;

  while (NL.empty() == false) {
    int n = rand() % NL.size();

    switch (NL[n]) {
    case 1:
      improved = Swap(s, data, subseq_matrix);
      break;

    case 2:
      improved = twoOpt(s, data, subseq_matrix);
      break;

    case 3:
      improved = orOpt(s, data, subseq_matrix, 1);
      break;

    case 4:
      improved = orOpt(s, data, subseq_matrix, 2);
      break;

    case 5:
      improved = orOpt(s, data, subseq_matrix, 3);
      break;
    }

    if (improved) {
      NL = {1, 2, 3, 4, 5};
    } else {
      NL.erase(NL.begin() + n);
    }
  }
}

Solution perturbacao(Solution s, Data &data) {

  int sizi, sizj;
  int V = data.getDimension();
  int max;
  if (V > 30) {
    max = V / 10;
  } else {
    max = 2;
  }
  sizi = 2 + rand() % (max - 2 + 1);
  sizj = 2 + rand() % (max - 2 + 1);
  int maxPositionI = (s.sequence.size() - 1) - sizi;
  int maxPositionJ = (s.sequence.size() - 1) - sizj;
  int i = 1 + rand() % (maxPositionI);
  int j = 1 + rand() % (maxPositionJ);

  int k = 0;
  int auxi = i;
  int auxj = j;
  bool teste = false;

  while (true) {
    teste = false;

    for (k = 0; k < sizi; k++) {
      auxj = j;

      for (int n = 0; n < sizj; n++) {
        if (auxi == auxj) {
          teste = true;
          break;
        }
        auxj++;
      }
      if (teste) {
        break;
      }
      auxi++;
    }
    if (teste) {
      j = 1 + rand() % (maxPositionJ);
      auxi = i;
    }

    else {
      break;
    }
  }

  int lastI = i + (sizi - 1);
  int lastJ = j + (sizj - 1);
  int menor, maior, lastMaior, lastMenor;

  if (j > i) {
    maior = j;
    lastMaior = lastJ;
    menor = i;
    lastMenor = lastI;
  } else {
    maior = i;
    lastMaior = lastI;
    menor = j;
    lastMenor = lastJ;
  }

  int n = 1;

  for (maior; maior <= lastMaior; maior++) {

    int vmaior = s.sequence[maior];
    s.sequence.erase(s.sequence.begin() + maior);
    s.sequence.insert(s.sequence.begin() + lastMenor + n, vmaior);
    n++;
  }

  for (int p = menor; p <= lastMenor; p++) {

    int vmenor = s.sequence[menor];
    s.sequence.erase(s.sequence.begin() + menor);
    s.sequence.insert(s.sequence.begin() + lastMaior, vmenor);
  }

  return s;
}

Solution ILS(int maxIter, int maxIterIls, Data &data,
             vector<vector<Subsequence>> &subseq_matrix) {
  Solution bestOfAll;
  bestOfAll.valorobj = 999999999999999;
  for (int i = 0; i < maxIter; i++) {
    Solution s = Construcao(data);
    UpdateAllSubseq(&s, subseq_matrix, data, 0, sizeof(s));
    Solution best = s;
    int iterIls = 0;

    while (iterIls <= maxIterIls) {
      buscaLocal(s, data, subseq_matrix);
      if (s.valorobj < best.valorobj) {
        best = s;
        iterIls = 0;
      }
      s = perturbacao(best, data);
      UpdateAllSubseq(&s, subseq_matrix, data, 0, sizeof(s));
      iterIls++;
    }

    if (best.valorobj < bestOfAll.valorobj) {
      bestOfAll = best;
    }
  }
  return bestOfAll;
}

int main(int argc, char **argv) {
  srand(time(NULL));

  auto data = Data(argc, argv[1]);
  data.read();
  size_t n = data.getDimension();

  int maxIter = 10;
  int V = data.getDimension();
  int maxIterIls;

  if (V >= 100) {
    maxIterIls = 100;
  } else {
    maxIterIls = V;
  }

  Solution s;

  auto inicio = high_resolution_clock::now();
  double valor = 0;

  for (int y = 0; y < 10; y++) {
    auto subseq_matrix =
        vector<vector<Subsequence>>(n + 1, vector<Subsequence>(n + 1));
    s = ILS(maxIter, maxIterIls, data, subseq_matrix);
    valor += s.valorobj;
  }

  auto fim = high_resolution_clock::now();

  // Calcula a duração
  auto tempo = duration_cast<microseconds>(fim - inicio);

  cout << (valor / 10) << " " << (tempo.count() / 1e7) << endl;

  return 0;
}