#include <iostream>
#include <vector>

#include "logLib.hpp"
#include "BVH_solution/solution.hpp"

int main() {
  BVH_fast_solution_t<double> BVH_solution;
  BVH_solution.input();
  std::vector<std::size_t> indices =
    BVH_solution.get_inter_triangs_indices();

  for (std::size_t ind : indices) {
    std::cout << ind << '\n';
  }
  std::cout.flush();

  return 0;
}

/*

3
-1 1 0 1 1 0 0 -1 0
0 1 0 -1 -1 0 1 -1 0
100 100 100 100 100 100

*/
