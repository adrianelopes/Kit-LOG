#include "BnB.h"
#include <iostream>
#include <vector>
#include <set>
#include <deque>
#include <algorithm>
#include <queue>
#include <iomanip>

using namespace std;

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
