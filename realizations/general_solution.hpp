#pragma once

#include "triangle.hpp"

template<typename T>
class general_solution_t {
 public:
  using triangs_list_t = std::vector<triangle_t<T>>;

  general_solution_t() = default;

  general_solution_t(const triangs_list_t& triangs)
      : num_triangs_(triangs.size()), triangs_(triangs) {}

  ~general_solution_t() = default;

  virtual std::vector<std::size_t> get_inter_triangs_indices() = 0;

  void input() {
    std::cin >> num_triangs_;
    triangs_.resize(num_triangs_);
    for (auto& triangle : triangs_) {
      std::cin >> triangle;
    }
  }

  // prevent from copying and assigning
  general_solution_t(const general_solution_t& other) = delete;
  general_solution_t& operator=(const general_solution_t& other) = delete;

 protected:
  std::size_t num_triangs_;
  std::vector<triangle_t<T>> triangs_;
};