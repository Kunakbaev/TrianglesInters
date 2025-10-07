#pragma once

#include "triangle.hpp"
#include "../general_solution.hpp"

template<typename T=double>
class bruteforce_solution_t : public general_solution_t<T> {
 public:
  using typename general_solution_t<T>::triangs_list_t;

  bruteforce_solution_t() = default;

  bruteforce_solution_t(const triangs_list_t& triangs)
      : general_solution_t<T>(triangs) {}

  std::vector<std::size_t> get_inter_triangs_indices() override {
    std::vector<std::size_t> result;
    std::vector<bool> is_marked(this->num_triangs_);
    for (std::size_t cur_ind = 0; cur_ind < this->num_triangs_; ++cur_ind) {
      if (is_marked[cur_ind]) {
        result.emplace_back(cur_ind);
        continue;
      }

      bool is_inter = false;
      const auto& cur = this->triangs_[cur_ind];
      for (std::size_t other_ind = 0; other_ind < this->num_triangs_; ++other_ind) {
        if (other_ind == cur_ind) {
          continue;
        }

        if (cur.does_intersect(this->triangs_[other_ind])) {
          result.emplace_back(cur_ind);
          is_marked[other_ind] = true;
          is_inter = true;
          break;
        }
      }
    }

    return result;
  }
};