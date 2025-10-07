#include <iostream>
#include <vector>

#include "logLib.hpp"
#include "bruteforce_solution/bruteforce_solution.hpp"

int main() {
  bruteforce_solution_t<double> brute_force_sol;
  brute_force_sol.input();
  std::vector<std::size_t> indices =
    brute_force_sol.get_inter_triangs_indices();

  for (std::size_t ind : indices) {
    std::cout << ind << '\n';
  }
  std::cout.flush();

  return 0;
}

/*

2
-1 1 0 1 1 0 0 -1 0
0 1 0 -1 -1 0 1 -1 0

*/
