#pragma once

#include <array>
#include <iostream>
#include <string_view>
#include <vector>

#include "plane.hpp"
#include "point.hpp"
#include "AABB.hpp"

template<typename T>
class triangle_with_box_t {
 public:
  triangle_with_box_t(const point_t<T>& a, const point_t<T>& b, const point_t<T>& c)
    : triangle_(a, b, c), bounding_box_(triangle_) {}

  triangle_with_box_t(const triangle_t<T>& triangle)
    : triangle_(triangle), bounding_box_(triangle) {}

  [[nodiscard]] AABB_t<T> get_AABB() const {
    return bounding_box_;
  }

  [[nodiscard]] point_t<T> get_center() const {
    return (bounding_box_.get_max_corner() +
            bounding_box_.get_min_corner()) * static_cast<T>(0.5);
  }

  [[nodiscard]] point_t<T> get_centroid() const {
    std::array<point_t<T>, 3> points = triangle_.get_points();
    return (points[0] + points[1] + points[2]) * static_cast<T>(0.5);
  }

  [[nodiscard]] bool does_intersect(const triangle_with_box_t& other) const {
    return triangle_.does_intersect(other.triangle_);
  }

 private:
  triangle_t<T> triangle_;
  AABB_t<T>  bounding_box_;
};
