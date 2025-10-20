#pragma once

#include "triangle_fwd.hpp"
#include "point.hpp"

// axis aligned bounding box class for triangle
template<typename T=double>
class AABB_t {
 public:
  AABB_t() = default;
  AABB_t(const triangle_t<T>& triangle);
  AABB_t(const point_t<T>& corner_min, const point_t<T>& corner_max);

  [[nodiscard]] bool does_inter(const AABB_t& other) const;

  [[nodiscard]] point_t<T> get_min_corner() const;
  [[nodiscard]] point_t<T> get_max_corner() const;

  void unite_with(const AABB_t& other);

  [[nodiscard]] utils::axis_t get_longest_axis_ind() const;

  [[nodiscard]] T get_volume() const;

  [[nodiscard]] AABB_t get_intersection(const AABB_t& other) const;
  
 private:
  [[nodiscard]] bool is_point_inside(const point_t<T>& point) const;

 private:
  point_t<T> corner_min_;
  point_t<T> corner_max_;
};

template<typename T>
AABB_t<T>::AABB_t(const triangle_t<T>& triangle) {
  std::array<point_t<T>, 3> points = triangle.get_points();
  corner_min_ = corner_max_ = points.front();

  // std::cerr << "bruh tire : " << triangle << std::endl;
  // for (auto& it : points) std::cerr << it << ", ";
  // std::cerr << std::endl;
  for (std::size_t i = 1; i < points.size(); ++i) {
    corner_min_ = vec_ops::get_min_of_2_points(corner_min_, points[i]);
    corner_max_ = vec_ops::get_max_of_2_points(corner_max_, points[i]);
    // std::cout << "corner min : " << corner_min_ << " max : " << corner_max_<< std::endl;
  }
  //exit(2);
}

template <typename T>
AABB_t<T>::AABB_t(const point_t<T>& corner_min, const point_t<T>& corner_max)
  : corner_min_(corner_min), corner_max_(corner_max) {}

template<typename T>
[[nodiscard]] inline bool AABB_t<T>::does_inter(const AABB_t<T>& other) const {
  if (utils::sign(corner_min_.x - other.corner_max_.x) == utils::signs_t::POS ||
      utils::sign(corner_max_.x - other.corner_min_.x) == utils::signs_t::NEG)
        return false;
  if (utils::sign(corner_min_.y - other.corner_max_.y) == utils::signs_t::POS ||
      utils::sign(corner_max_.y - other.corner_min_.y) == utils::signs_t::NEG)
        return false;
  if (utils::sign(corner_min_.z - other.corner_max_.z) == utils::signs_t::POS ||
      utils::sign(corner_max_.z - other.corner_min_.z) == utils::signs_t::NEG)
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
  T len_x = corner_max_.x - corner_min_.x;
  T len_y = corner_max_.y - corner_min_.y;
  T len_z = corner_max_.z - corner_min_.z;

  if (utils::sign(len_x - len_y) >= utils::signs_t::ZERO &&
      utils::sign(len_x - len_z) >= utils::signs_t::ZERO) {
    return utils::axis_t::X;
  }

  // x is not the biggest, we need to compare y and z
  if (utils::sign(len_y - len_z) >= utils::signs_t::ZERO) {
    return utils::axis_t::Y;
  }

  return utils::axis_t::Z;
}

// TODO: rename to surface area
template <typename T>
[[nodiscard]] T AABB_t<T>::get_volume() const {
  T len_x = corner_max_.x - corner_min_.x;
  T len_y = corner_max_.y - corner_min_.y;
  T len_z = corner_max_.z - corner_min_.z;

  T volume = 2 * (len_x * len_y + len_x * len_z + len_y * len_z);
  // std::cerr << "volume : " << volume << " lenx : " <<
  // len_x << " leny : " << len_y << " lenz : " << len_z << "\n";
  return volume;
}

template <typename T>
[[nodiscard]] bool AABB_t<T>::is_point_inside(const point_t<T>& point) const {
  // TODO: BRUH:
  return does_inter({point, point});
}

template <typename T>
[[nodiscard]] AABB_t<T> AABB_t<T>::get_intersection(const AABB_t& other) const {
  if (!does_inter(other)) {
    return {};
  }

  if (is_point_inside(other.get_min_corner())) {
    return {other.get_min_corner(), get_max_corner()};
  } else {
    return {get_min_corner(), other.get_max_corner()};
  }
}
