#pragma once

#include "triangle.hpp"
#include "../general_solution.hpp"
#include "BVH.hpp"

template<typename T>
class BVH_fast_solution_t : public general_solution_t<T> {
 public:
  using typename general_solution_t<T>::triangs_list_t;

  BVH_fast_solution_t() = default;

  BVH_fast_solution_t(const triangs_list_t& triangs)
      : general_solution_t<T>(triangs) {}

  std::vector<std::size_t> get_inter_triangs_indices() override {
    BVH_t BVH_tree(this->triangs_);
    std::vector<std::size_t> result;
    // std::vector<bool> is_marked(num_triangs_);
    for (std::size_t cur_ind = 0; cur_ind < this->num_triangs_; ++cur_ind) {
      if (BVH_tree.is_triangle_not_alone(
          this->triangs_[cur_ind], cur_ind)) {
        result.emplace_back(cur_ind);
      }
    }

    return result;

    // auto bvh_root = build_BVH<double>(this->triangs_);
    // std::vector<std::size_t> result;
    // std::vector<bool> is_marked(this->triangs_.size());
    // for (std::size_t cur_ind = 0; cur_ind < this->num_triangs_; ++cur_ind) {
    //   if (is_triangle_not_alone<double>(
    //     bvh_root.get(), this->triangs_, this->triangs_[cur_ind], cur_ind, is_marked)) {
    //     result.emplace_back(cur_ind);
    //   }
    // }

    // return result;
  }
};