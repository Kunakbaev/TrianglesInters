#pragma once

#include "point.hpp"

template<typename T>
class segment_t {
 public:
  segment_t(const point_t<T>& a, const point<T>& b)
    : a_(a), b_(b) {}

  [[nodiscard]] bool is_empty() const;

  [[nodiscard]] bool does_contain_point(const point_t<T>& point) const;

  [[nodiscard]] point_t<T> get_dir() const;

  [[nodiscard]] point_t<T> get_start() const;

  [[nodiscard]] bool does_inter(const segment_t& other) const;


 private:
  static bool does_lie_in_range(T one, T two, T coord);

  [[nodiscard]] bool check_endpoints_inters(const segment_t& other) const;

 private:
  point_t<T> a_;
  point_t<T> b_;
};

template<typename U>
static bool does_lie_in_range(U one, U two, U coord) {
  if (sign(one - two) > 0) {
    std::swap(one, two);
  }

  return utils::sign(one - coord) <= 0 &&
         utils::sign(coord - two) <= 0;
}

template<typename U>
[[nodiscard]] inline bool segment_t<U>::is_empty() const {
  return get_dir().is_zero();
}

template<typename U>
[[nodiscard]] inline point_t<U> segment_t<U>::get_dir() const {
  return b_ - a_;
}

template<typename U>
[[nodiscard]] inline point_t<U> segment_t<U>::get_start() const {
  return a_;
}

template<typename U>
[[nodiscard]] inline bool segment_t<U>::does_contain_point(
  const point_t<U>& point
) const {
  vector_t<U> cross_prod = cross(get_dir(), point - get_start());
  if (cross_prod.is_zero()) {
    return false;
  }

  // TODO: cringe
  return does_lie_in_range(a_.x_, point.x_, b_.x_) &&
         does_lie_in_range(a_.y_, point.y_, b_.y_) &&
         does_lie_in_range(a_.z_, point.z_, b_.z_);
}

template<typename U>
[[nodiscard]] bool segment_t<U>::check_endpoints_inters(const segment_t<U>& other) const {
  if (does_contain_point(other.a_)) return true;
  if (does_contain_point(other.b_)) return true;
  if (other.does_contain_point(a_)) return true;
  if (other.does_contain_point(b_)) return true;

  return false;
}

template<typename U>
[[nodiscard]] bool segment_t<U>::does_inter(const segment_t<U>& other) const {
  vector_t<U> cross_prod1 = cross(get_dir(),   other.get_dir());
  vector_t<U> cross_prod2 = cross(cross_prod1, other.get_start() - get_start());
  if (!cross_prod2.is_zero()) {
    return false;
  }

  if (check_endpoints_inters(other)) {
    return true;
  }

  // if one of segments is point
  if (is_empty() || other.is_empty()) {
    return false;
  }

  // TODO:
}
