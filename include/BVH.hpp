#pragma once

#include <algorithm>
#include <numeric>
#include <memory>

#include "logLib.hpp"
#include "triangle_with_box.hpp"

template<typename T>
class BVH_t {
 private:
  class node_t;
 public:
  using triangs_list_t = std::vector<triangle_with_box_t<T>>;
  using indices_list_t = std::vector<std::size_t>;

  BVH_t(const std::vector<triangle_t<T>>& triangles)
      : num_triangles_(triangles.size()),
        triangles_(triangles.begin(), triangles.end()),
        visited_(num_triangles_) {
    indices_list_t indices(num_triangles_);
    std::iota(indices.begin(), indices.end(), 0);
    root_ = construct_BVH_tree(indices, 0);
  }

  [[nodiscard]] bool is_triangle_not_alone(
    const triangle_with_box_t<T>& triangle,
    std::size_t                   triangle_ind
  );

 private:
  AABB_t<T> find_bounding_box4triangs(
    const indices_list_t& indices
  ) const;

  void partition_triangles(
    const AABB_t<T>&      box,
    const indices_list_t& indices,
    indices_list_t&       lhs,
    indices_list_t&       rhs
  );

  void partition_triangles_by_ort_to_axis(
    utils::axis_t         axis_name,
    const AABB_t<T>&      box,
    const indices_list_t& indices,
    indices_list_t&       lhs,
    indices_list_t&       rhs
  );

  [[nodiscard]] std::unique_ptr<node_t> construct_BVH_tree(
    const indices_list_t& indices,
    std::size_t           depth
  );

  [[nodiscard]] bool is_triangle_not_alone_rec(
    const node_t*                 cur_node,
    const triangle_with_box_t<T>& triangle,
    std::size_t                   triangle_ind
  );

 private:
  class node_t {
   private:
    std::unique_ptr<node_t>         left_    = nullptr;
    std::unique_ptr<node_t>         right_   = nullptr;
    std::unique_ptr<indices_list_t> indices_ = {};

   public:
    AABB_t<T> box     = {};
    bool      is_leaf = false;

    // constructor for NON leaf node
    node_t(const AABB_t<T>& box_, std::unique_ptr<node_t> left,
           std::unique_ptr<node_t> right)
        : box(box_), left_(std::move(left)), right_(std::move(right)) {}
    // constructor for leaf node
    node_t(const AABB_t<T>& box, indices_list_t indices)
        : is_leaf(true), box(box),
          indices_(std::make_unique<indices_list_t>(std::move(indices))) {}

    const node_t*  left() const { return left_.get(); }
    const node_t* right() const { return right_.get(); }
    const indices_list_t& get_indices() const { return *indices_; }
  };

 private:
  const node_t* get_root() const { return root_.get(); }

 private:
  static const std::size_t kLeafNumOfTriangles = 8;

 private:
  const std::size_t       num_triangles_;
  const triangs_list_t    triangles_;
  std::unique_ptr<node_t> root_ = nullptr;
  std::vector<int>        visited_;
};

template <typename T>
[[nodiscard]] typename std::unique_ptr<typename BVH_t<T>::node_t> BVH_t<T>::construct_BVH_tree(
  const indices_list_t& indices, std::size_t depth
) {
  AABB_t box = find_bounding_box4triangs(indices);
  bool is_leaf = indices.size() <= kLeafNumOfTriangles;
  indices_list_t lhs;
  indices_list_t rhs;

  if (!is_leaf) {
    partition_triangles(box, indices, lhs, rhs);

    // TODO: redundant aabb search
    AABB_t left_box  = find_bounding_box4triangs(lhs);
    AABB_t right_box = find_bounding_box4triangs(rhs);
    T inter_volume = left_box.get_intersection(right_box).get_volume();
    if ((lhs.empty() || rhs.empty()) ||
      (depth >= 8  && inter_volume > static_cast<T>(0.3) * box.get_volume()) ||
      (depth >= 12 && inter_volume > static_cast<T>(0.1) * box.get_volume()) ||
       depth >= 14) {
      is_leaf = true;
    }
  }

  if (is_leaf) {
    auto cur_node = std::make_unique<node_t>(box, std::move(indices));
    return cur_node;
  }

  std::unique_ptr<node_t> left  = construct_BVH_tree(lhs, depth + 1);
  std::unique_ptr<node_t> right = construct_BVH_tree(rhs, depth + 1);
  auto cur_node = std::make_unique<node_t>(box, std::move(left), std::move(right));
  return cur_node;
}

template <typename T>
[[nodiscard]] bool BVH_t<T>::is_triangle_not_alone(
  const triangle_with_box_t<T>& triangle,
  std::size_t          triangle_ind
) {
  if (visited_[triangle_ind]) {
    return true;
  }

  bool is_not_alone = is_triangle_not_alone_rec(
    get_root(), triangle, triangle_ind
  );

  return is_not_alone;
}

template <typename T>
[[nodiscard]] bool BVH_t<T>::is_triangle_not_alone_rec(
  const node_t*                 cur_node,
  const triangle_with_box_t<T>& triangle,
  std::size_t                   triangle_ind
) {
  if (!cur_node->box.does_inter(triangle.get_AABB())) {
    return false;
  }

  if (cur_node->is_leaf) {
    for (auto& ind : cur_node->get_indices()) {
      if (!triangles_[ind].get_AABB().does_inter(triangle.get_AABB())) {
        continue;
      }

      if (triangles_[ind].does_intersect(triangle) && 
          ind != triangle_ind) {
        visited_[ind] = visited_[triangle_ind] = true;
        // we don't want count triangle intersection with itself
        return true;
      }
    }

    return false;
  }

  bool is_inter = is_triangle_not_alone_rec(cur_node->left(), triangle, triangle_ind);
  if (!is_inter) {
    is_inter = is_triangle_not_alone_rec(cur_node->right(), triangle, triangle_ind);
  }

  return is_inter;
}

template <typename T>
[[nodiscard]] AABB_t<T> BVH_t<T>::find_bounding_box4triangs(
  const indices_list_t& indices
) const {
  AABB_t<T> box;
  if (indices.empty()) {
    return box;
  }

  box = triangles_[indices.front()].get_AABB();
  for (std::size_t i = 1; i < indices.size(); ++i) {
    box.unite_with(triangles_[indices[i]].get_AABB());
  }

  return box;
}

template <typename U>
inline void BVH_t<U>::partition_triangles(
  const AABB_t<U>&      box,
  const indices_list_t& indices,
  indices_list_t&       lhs,
  indices_list_t&       rhs
) {
  const std::vector<utils::axis_t> axes = {
    utils::axis_t::X,
    utils::axis_t::Y,
    utils::axis_t::Z
  };

  indices_list_t lhs2;
  indices_list_t rhs2;
  U best_cost = std::numeric_limits<U>::max();
  for (auto axis : axes) {
    partition_triangles_by_ort_to_axis(
      axis, box, indices, lhs2, rhs2
    );

    AABB_t left_box  = find_bounding_box4triangs(lhs2);
    AABB_t right_box = find_bounding_box4triangs(rhs2);
    U cost = left_box.get_volume() * lhs2.size() + right_box.get_volume() * rhs2.size();
    if (cost < best_cost) {
      best_cost = cost;
      std::swap(lhs, lhs2);
      std::swap(rhs, rhs2);
    }
  }
}

template <typename U>
inline void BVH_t<U>::partition_triangles_by_ort_to_axis(
  utils::axis_t         axis_name,
  const AABB_t<U>&      box,
  const indices_list_t& indices,
  indices_list_t&       lhs,
  indices_list_t&       rhs
) {
  // setLoggingLevel(DEBUG);
  // utils::axis_t longest_axis_ind = box.get_longest_axis_ind();
  // LOG_DEBUG_VARS(static_cast<std::size_t>(longest_axis_ind));
  // std::cerr << "axis : "  << static_cast<std::size_t>(longest_axis_ind) << std::endl;

  using proj_pair_t = std::pair<U, std::size_t>;
  std::vector<proj_pair_t> projection_pairs;
  for (auto& ind : indices) {
    U coord = triangles_[ind].get_center().get_coord_by_axis_name(axis_name);
    projection_pairs.emplace_back(coord, ind);
  }

  auto cmp_proj_pairs = [](const proj_pair_t& lhs, const proj_pair_t& rhs){
    return utils::sign(lhs.first - rhs.first) < utils::signs_t::ZERO;
  };
  const std::size_t mid_ind = indices.size() / 2;
  std::nth_element(projection_pairs.begin(),
                   projection_pairs.begin() + mid_ind,
                   projection_pairs.end(),
                   cmp_proj_pairs);
  U partition_coord = projection_pairs[mid_ind].first;

  lhs.clear();
  rhs.clear();
  for (auto& [proj_coord, ind] : projection_pairs) {
    if (utils::sign(proj_coord - partition_coord) <= utils::signs_t::ZERO) {
      lhs.emplace_back(ind);
    } else {
      rhs.emplace_back(ind);
    }
  }
}
