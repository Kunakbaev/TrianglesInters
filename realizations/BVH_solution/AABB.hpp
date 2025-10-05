#pragma once

#include "triangle.hpp"

// axis aligned bounding box class for triangle
template<typename T=double>
class AABB_t {
 public:
  AABB_t(const triangle_t<T>& triangle);

  [[nodiscard]] bool does_inter(const AABB_t& other) const;

  [[nodiscard]] point_t<T> get_min_corner() const;
  [[nodiscard]] point_t<T> get_max_corner() const;

 private:
  point_t<T> corner_min_;
  point_t<T> corner_max_;
};

template<typename T>
inline AABB_t<T>::AABB_t(const triangle_t<T>& triangle)
    : corner_min_(triangle.a_), corner_max_(triangle.a_) {
  std::vector<point_t<T>> points = {
    triangle.b_, triangle.c_
  };

  for (const point_t<T>& point : points) {
    corner_min_ = corner_min_.get_min_point(point);
    corner_max_ = corner_max_.get_max_point(point);
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
