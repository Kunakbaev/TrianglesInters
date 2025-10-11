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
      // assert(!this->triangs_[cur_ind].is_valid());
      // std::cerr << "cur_ind : " << cur_ind << " triang : " << this->triangs_[cur_ind] << "\n";
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

    // std::vector<std::size_t> corr;
    // for (std::size_t i = 0; i < this->triangs_.size(); ++i) {
    //   for (std::size_t k = 0; k < this->triangs_.size(); ++k) {
    //     if (i == k) continue;
    //     triangle_t<T> one = this->triangs_[i];
    //     triangle_t<T> two = this->triangs_[k];
    //     bool is_eq = true;
    //     std::vector<point_t<T>> aa = one.get_points();
    //     std::vector<point_t<T>> bb = two.get_points();
    //     for (std::size_t j = 0; j < 3; ++j) {
    //       //std::cerr << "a : " << aa[j] << " " << bb[j] << std::endl;
    //       if (!(aa[j] - bb[j]).is_zero()) {
    //         // std::cout << "I : " << i << std::endl;
    //         is_eq = false;
    //         break;
    //       }
    //     }

    //     if (is_eq) {
    //       corr.emplace_back(i);
    //     }
    //   }
    // }

    // if (corr != result) {
    //   std::cout << "Bruh" << std::endl; 
    //   assert(false);
    // }

    return result;
  }
};