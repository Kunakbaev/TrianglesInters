#pragma once

#include <algorithm>

#include "logLib.hpp"
#include "triangle.hpp"

template<typename T>
class BVH_t {
 private:
  class node_t;
 public:
  struct triang_with_ind_t {
    triangle_t<T> triang;
    std::size_t   ind;
    triang_with_ind_t() = default;
    triang_with_ind_t(const triangle_t<T>& triang_, std::size_t ind_)
        : triang(triang_), ind(ind_) {}
  };

  using triangs_list_t = std::vector<triangle_t<T>>;
  using triangs_with_ind_list_t = std::vector<triang_with_ind_t>;

  BVH_t(const triangs_list_t& triangles) {
    triangs_with_ind_list_t arr;
    for (std::size_t i = 0; i < triangles.size(); ++i) {
      arr.emplace_back(triangles[i], i);
    }
    root_ = construct_BVH_tree(arr);
  }

  [[nodiscard]] bool is_triangle_not_alone(
    const triangle_t<T>& triangle,
    std::size_t          triangle_ind
  );

 private:
  AABB_t<T> find_bounding_box4triangs(
    const triangs_with_ind_list_t& triangles
  ) const;

  void partition_triangles(
    const AABB_t<T>&               box,
    const triangs_with_ind_list_t& triangles,
    triangs_with_ind_list_t&       lhs,
    triangs_with_ind_list_t&       rhs
  );

  [[nodiscard]] node_t* construct_BVH_tree(
    const triangs_with_ind_list_t& triangles
  );

  [[nodiscard]] bool is_triangle_not_alone_rec(
    const node_t*        cur_node,
    const triangle_t<T>& triangle,
    std::size_t          triangle_ind
  );

 private:
  class node_t {
   public:
    node_t*                 left    = nullptr;
    node_t*                 right   = nullptr;
    AABB_t<T>               box     = {};
    bool                    is_leaf = false;
    triangs_with_ind_list_t triangs = {}; // ASK: this field is only needed in leaves,
                                 // half the time we store redundant

    // constructor for leaf
    node_t(const AABB_t<T>& box_,
           const triangs_with_ind_list_t& triangles)
        : is_leaf(true), box(box_), triangs(triangles) {}
    // constructor for NON leaf node
    node_t(const AABB_t<T>& box_,
           node_t* left_, node_t* right_)
        : box(box_), left(left_), right(right_) {}
  };
 
 private:
  static const std::size_t kLeaveNumOfTriangles = 4;

 private:
  node_t* root_ = nullptr;
};

template <typename T>
[[nodiscard]] typename BVH_t<T>::node_t* BVH_t<T>::construct_BVH_tree(
  const triangs_with_ind_list_t& triangles
) {
  AABB_t box = find_bounding_box4triangs(triangles);
  bool is_leaf = triangles.size() <= kLeaveNumOfTriangles;
  triangs_with_ind_list_t lhs;
  triangs_with_ind_list_t rhs;

  if (!is_leaf) {
    partition_triangles(box, triangles, lhs, rhs);
    is_leaf |= lhs.empty() || rhs.empty();
  }

  if (is_leaf) {
    node_t* cur_node = new node_t(box, triangles);
    // if (triangles.size() > 5) {
    //   std::cout << "leaf size : " << triangles.size() << std::endl;
    // }
    return cur_node;
  }

  // std::cout << "success partition" << std::endl;
  node_t* left  = construct_BVH_tree(lhs);
  node_t* right = construct_BVH_tree(rhs);
  node_t* cur_node = new node_t(box, left, right);
  return cur_node;
}

template <typename T>
[[nodiscard]] bool BVH_t<T>::is_triangle_not_alone(
  const triangle_t<T>& triangle,
  std::size_t          triangle_ind
) {
  bool is_not_alone = is_triangle_not_alone_rec(
    root_, triangle, triangle_ind
  );

  return is_not_alone;
}

template <typename T>
[[nodiscard]] bool BVH_t<T>::is_triangle_not_alone_rec(
  const node_t*        cur_node,
  const triangle_t<T>& triangle,
  std::size_t          triangle_ind
) {
  if ( cur_node == nullptr ||
    !cur_node->box.does_inter(triangle.get_AABB())
  ) {
    return false;
  }

  if (cur_node->is_leaf) {
    std::size_t cnt = 0;
    for (auto& [node_triang, ind] : cur_node->triangs) {
      if (node_triang.does_intersect(triangle) && 
          ind != triangle_ind) {
        // we don't want count triangle intersection with itself
        return true;
      }
    }

    return false;
  }

  bool is_inter = is_triangle_not_alone_rec(cur_node->left, triangle, triangle_ind);
  if (!is_inter) {
    is_inter = is_triangle_not_alone_rec(cur_node->right, triangle, triangle_ind);
  }

  return is_inter;
}

template <typename T>
[[nodiscard]] AABB_t<T> BVH_t<T>::find_bounding_box4triangs(
  const triangs_with_ind_list_t& triangles
) const {
  AABB_t<T> box;
  if (triangles.empty()) {
    return box;
  }

  box = triangles.front().triang.get_AABB();
  for (std::size_t i = 1; i < triangles.size(); ++i) {
    box.unite_with(triangles[i].triang.get_AABB());
  }

  return box;
}

template <typename U>
inline void BVH_t<U>::partition_triangles(
  const AABB_t<U>&               box,
  const triangs_with_ind_list_t& triangles,
  triangs_with_ind_list_t&       lhs,
  triangs_with_ind_list_t&       rhs
) {
  setLoggingLevel(DEBUG);
  utils::axis_t longest_axis_ind = box.get_longest_axis_ind();
  LOG_DEBUG_VARS(static_cast<std::size_t>(longest_axis_ind));
  // std::cerr << "axis : "  << static_cast<std::size_t>(longest_axis_ind) << std::endl;

  using proj_pair_t = std::pair<U, std::size_t>;
  std::vector<proj_pair_t> projection_pairs;
  std::size_t triang_ind = 0;
  for (auto& [triangle, _] : triangles) {
    AABB_t triang_box = triangle.get_AABB();
    // std::cerr << "center : " << (triang_box.get_max_corner() +
    //    triang_box.get_min_corner()) << " " << triang_box.get_max_corner() << " " << triang_box.get_min_corner() << std::endl;
    point_t<U> center =
      (triang_box.get_max_corner() +
       triang_box.get_min_corner()) * static_cast<U>(0.5);
    U coord = center.get_coord_by_axis_name(longest_axis_ind);
    projection_pairs.emplace_back(coord, triang_ind);
    // std::cerr << "ind : " << _ << " coord : " << coord << " triang : " << triangle << std::endl;



    ++triang_ind;
  }

  auto cmp_proj_pairs = [](const proj_pair_t& lhs, const proj_pair_t& rhs){
    return utils::sign(lhs.first - rhs.first) < 0;
  };
  const std::size_t mid_ind = triangles.size() / 2;
  std::nth_element(projection_pairs.begin(),
                   projection_pairs.begin() + mid_ind,
                   projection_pairs.end(),
                   cmp_proj_pairs);
  U partition_coord = projection_pairs[mid_ind].first;
  // std::cerr << "partition coord : " << partition_coord << std::endl;

  for (auto& [proj_coord, ind] : projection_pairs) {
    if (utils::sign(proj_coord - partition_coord) <= 0) {
      lhs.emplace_back(triangles[ind]);
    } else {
      rhs.emplace_back(triangles[ind]);
    }
  }
}