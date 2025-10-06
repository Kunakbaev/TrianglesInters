#pragma once

#include "triangle_fwd.hpp"
#include "point.hpp"

// axis aligned bounding box class for triangle
template<typename T=double>
class AABB_t {
 public:
  AABB_t() = default;
  AABB_t(const triangle_t<T>& triangle);

  [[nodiscard]] bool does_inter(const AABB_t& other) const;

  [[nodiscard]] point_t<T> get_min_corner() const;
  [[nodiscard]] point_t<T> get_max_corner() const;

  void unite_with(const AABB_t& other);

  [[nodiscard]] utils::axis_t get_longest_axis_ind() const;
  
 private:
  point_t<T> corner_min_;
  point_t<T> corner_max_;
};

template<typename T>
AABB_t<T>::AABB_t(const triangle_t<T>& triangle) {
  std::vector<point_t<T>> points = triangle.get_points();
  corner_min_ = corner_max_ = points.front();

  for (std::size_t i = 1; i < points.size(); ++i) {
    corner_min_ = vec_ops::get_min_of_2_points(corner_min_, points[i]);
    corner_max_ = vec_ops::get_max_of_2_points(corner_max_, points[i]);
  }
}

template<typename T>
[[nodiscard]] inline bool AABB_t<T>::does_inter(const AABB_t<T>& other) const {
  if (utils::sign(corner_min_.get_x() - other.corner_max_.get_x()) > 0 ||
      utils::sign(corner_max_.get_x() - other.corner_min_.get_x()) < 0)
        return false;
  if (utils::sign(corner_min_.get_y() - other.corner_max_.get_y()) > 0 ||
      utils::sign(corner_max_.get_y() - other.corner_min_.get_y()) < 0)
        return false;
  if (utils::sign(corner_min_.get_z() - other.corner_max_.get_z()) > 0 ||
      utils::sign(corner_max_.get_z() - other.corner_min_.get_z()) < 0)
        return false;

  return true;
}

template<typename T>
[[nodiscard]] inline point_t<T> AABB_t<T>::get_min_corner() const { return corner_min_; }

template<typename T>
[[nodiscard]] inline point_t<T> AABB_t<T>::get_max_corner() const { return corner_max_; }

template<typename T>
void AABB_t<T>::unite_with(const AABB_t& other) {
  corner_min_ = vec_ops::get_min_of_2_points(corner_min_, other.corner_min_);
  corner_max_ = vec_ops::get_max_of_2_points(corner_max_, other.corner_max_);
}

template<typename T>
[[nodiscard]] utils::axis_t AABB_t<T>::get_longest_axis_ind() const {
  T len_x = corner_max_.get_x() - corner_min_.get_x();
  T len_y = corner_max_.get_y() - corner_min_.get_y();
  T len_z = corner_max_.get_z() - corner_min_.get_z();

  if (utils::sign(len_x - len_y) >= 0 && utils::sign(len_x - len_z) >= 0) {
    return utils::axis_t::X;
  }

  // x is not the biggest, we need to compare y and z
  if (utils::sign(len_y - len_z) >= 0) {
    return utils::axis_t::Y;
  }

  return utils::axis_t::Z;
}
