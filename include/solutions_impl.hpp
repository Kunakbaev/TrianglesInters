#pragma once

#include "triangle.hpp"
#include "BVH.hpp"

struct naive_solution_tag {};
struct opt_bvh_solution_tag {};

template<typename T, typename solution_tag>
class triangles_inters_solver_t {
 public:
  using triangs_list_t = std::vector<triangle_t<T>>;

 public:
  triangles_inters_solver_t() = default;

  triangles_inters_solver_t(const triangs_list_t& triangs)
      : num_triangs_(triangs.size()), triangs_(triangs) {}

  std::vector<std::size_t> get_inter_triangs_indices() {
    return solve_impl(solution_tag{});
  }

  void input() {
    std::cin >> num_triangs_;
    triangs_.resize(num_triangs_);
    for (auto& triangle : triangs_) {
      std::cin >> triangle;
    }
  }

  // prevent from copying and assigning
  triangles_inters_solver_t(const triangles_inters_solver_t& other) = delete;
  triangles_inters_solver_t& operator=(const triangles_inters_solver_t& other) = delete;

 private:
  // naive solution
  std::vector<std::size_t> solve_impl(
    naive_solution_tag
  ) {
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

  // fast solution, naive optimized with BVH tree
  std::vector<std::size_t> solve_impl(
    opt_bvh_solution_tag
  ) {
    BVH_t BVH_tree(triangs_);
    std::vector<std::size_t> result;
    for (std::size_t cur_ind = 0; cur_ind < num_triangs_; ++cur_ind) {
      if (BVH_tree.is_triangle_not_alone(
          triangs_[cur_ind], cur_ind)) {
        result.emplace_back(cur_ind);
      }
    }

    return result;
  }

 private:
  std::size_t num_triangs_;
  std::vector<triangle_t<T>> triangs_;
};