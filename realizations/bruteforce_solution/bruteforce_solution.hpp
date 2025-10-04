#pragma once

#include "triangle.hpp"

template<typename T>
class bruteforce_solution_t {
 public:
  using triangs_list_t = std::vector<triangle_t<T>>;

  bruteforce_solution_t() = default;

  bruteforce_solution_t(const triangs_list_t& triangs)
      : num_triangs_(triangs.size()), triangs_(triangs) {}

  std::vector<std::size_t> get_inter_triangs_indices() {
    std::vector<std::size_t> result;
    std::vector<bool> is_marked(num_triangs_);
    for (std::size_t cur_ind = 0; cur_ind < num_triangs_; ++cur_ind) {
      if (is_marked[cur_ind]) {
        result.emplace_back(cur_ind);
        continue;
      }

      bool is_inter = false;
      const auto& cur = triangs_[cur_ind];
      for (std::size_t other_ind = 0; other_ind < num_triangs_; ++other_ind) {
        if (other_ind == cur_ind) {
          continue;
        }

        if (cur.does_intersect(triangs_[other_ind])) {
          result.emplace_back(cur_ind);
          is_marked[other_ind] = true;
          is_inter = true;
          break;
        }
      }
    }

    return result;
  }

  void input() {
    std::cin >> num_triangs_;
    triangs_.reserve(num_triangs_);
    for (std::size_t i = 0; i < num_triangs_; ++i) {
      triangle_t<double> triangle;
      std::cin >> triangle;
      //std::cin >> triangle;
    }
  }

 private:
  std::size_t num_triangs_;
  std::vector<triangle_t<double>> triangs_;
};