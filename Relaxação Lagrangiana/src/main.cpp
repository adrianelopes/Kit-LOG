#include "BnB.h"
#include "grasp.h"
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

int main(int argc, char **argv)
{

  Data *data = new Data(argc, argv[1]);
  data->read();

  int tipo = stoi(argv[2]);

  /*   std::cout << std::setw(8) << "Node"
              << std::setw(8) << "Left"
              << std::setw(12) << "Tree"
              << std::setw(14) << "BestInt"
              << std::setw(14) << "BestBd"
              << std::setw(10) << "ItrCnt"
              << std::setw(10) << "Gap(%)"
              << "\n"; */

  double UB = grasp(*data);
  auto inicio = high_resolution_clock::now();
  double BnB = 0;

  for (int y = 0; y < 10; y++)
  {
    BnB += branch_and_bound(data, UB, tipo);
  }
  auto fim = high_resolution_clock::now();

  auto tempo = duration_cast<microseconds>(fim - inicio);

  cout << (BnB / 10) << " " << (tempo.count() / 1e7) << endl;

  delete data;

  return 0;
}
