#pragma once

#include <cassert>
#include <cmath>

#include "utils.hpp"

// template class, so it's easy to quickly change data type of coords without pain

template<typename T> class point_t;
template<typename T> using vector_t = point_t<T>;

namespace vec_ops {
  template<typename U>
  [[nodiscard]] point_t<U> get_max_of_2_points(
    const point_t<U>& lhs,
    const point_t<U>& rhs
  );

  template<typename U>
  [[nodiscard]] point_t<U> get_min_of_2_points(
    const point_t<U>& lhs,
    const point_t<U>& rhs
  );

  // dot product of 2 vectors
  template<typename U>
  [[nodiscard]] U dot(const point_t<U>& lhs, const point_t<U>& rhs);

  // cross product of 2 vectors
  template<typename U>
  [[nodiscard]] point_t<U> cross(const point_t<U>& lhs, const point_t<U>& rhs);

  template<typename U>
  [[nodiscard]] U mixed_prod(
    const vector_t<U>& a,
    const vector_t<U>& b,
    const vector_t<U>& c
  );
};

template<typename T=double>
class point_t {
 public:
  point_t() = default;

  point_t(T x, T y, T z) : x(x), y(y), z(z) {}

  [[nodiscard]] T get_len_sq() const;

  [[nodiscard]] T get_len() const;

  void norm() const;

  [[nodiscard]] bool is_zero() const;

  [[nodiscard]] point_t operator+(const point_t& other) const;
  [[nodiscard]] point_t operator-(const point_t& other) const;

  [[nodiscard]] T get_coord_by_axis_name(utils::axis_t axis) const;

  [[nodiscard]] point_t operator*(T coeff) const;

  [[nodiscard]] bool operator==(const vector_t<T>& other) const;

  [[nodiscard]] point_t& operator=(const point_t<T>& other) = default;

 public:
  T x{};
  T y{};
  T z{};
};

template<typename U>
[[nodiscard]] inline U point_t<U>::get_len_sq() const {
  return utils::sqr(x) + utils::sqr(y) + utils::sqr(z);
}

template<typename U>
[[nodiscard]] inline U point_t<U>::get_len() const {
  U len_sq = get_len_sq();
  return sqrt(len_sq); // WARNING: is it overloaded? Won't there be any precision loss?
  // if (std::is_same<T, float>::value) {
  //   return sqrt(len);
  // }
  // if (std::is_same<T, double>::value) {
  //   return sqrtl(len);
  // }
  // if (std::is_same<T, long double>::value) {
  //   return sqrtl(len);
  // }

  // assert(false);
}

template<typename U>
inline void point_t<U>::norm() const {
  U len = get_len();
  if (utils::sign(len) != 0) {
    x /= len;
    y /= len;
    z /= len;
  }
}

template<typename U>
[[nodiscard]] bool point_t<U>::is_zero() const {
  // ASK: maybe this is better: return utils::sign(get_len_sq()) == 0;
  return utils::sign(x) == utils::signs_t::ZERO &&
         utils::sign(y) == utils::signs_t::ZERO &&
         utils::sign(z) == utils::signs_t::ZERO;
}

template<typename U>
[[nodiscard]] inline point_t<U> point_t<U>::operator+(const point_t<U>& other) const {
  point_t<U> res(x + other.x, y + other.y, z + other.z);
  return res;
}

template<typename U>
[[nodiscard]] inline point_t<U> point_t<U>::operator-(const point_t<U>& other) const {
  point_t<U> res(x - other.x, y - other.y, z - other.z);
  return res;
}

template<typename U>
[[nodiscard]] U point_t<U>::get_coord_by_axis_name(utils::axis_t axis) const {
  switch (axis) {
    case utils::axis_t::X: return x;
    case utils::axis_t::Y: return y;
    case utils::axis_t::Z: return z;
    default:
      assert(false);
      std::cerr << "Error: invalid axis name..." << std::endl;
      return 0;
  }
}

namespace vec_ops {

template<typename U>
[[nodiscard]] point_t<U> get_max_of_2_points(
  const point_t<U>& lhs,
  const point_t<U>& rhs
) {
  point_t<U> res = lhs;
  if (utils::sign(rhs.x - lhs.x) == utils::signs_t::POS) res.x = rhs.x;
  if (utils::sign(rhs.y - lhs.y) == utils::signs_t::POS) res.y = rhs.y;
  if (utils::sign(rhs.z - lhs.z) == utils::signs_t::POS) res.z = rhs.z;
  return res;
}

template<typename U>
[[nodiscard]] point_t<U> get_min_of_2_points(
  const point_t<U>& lhs,
  const point_t<U>& rhs
) {
  point_t<U> res = lhs;
  if (utils::sign(rhs.x - lhs.x) == utils::signs_t::NEG) res.x = rhs.x;
  if (utils::sign(rhs.y - lhs.y) == utils::signs_t::NEG) res.y = rhs.y;
  if (utils::sign(rhs.z - lhs.z) == utils::signs_t::NEG) res.z = rhs.z;
  return res;
}

// dot product of 2 vectors
template<typename U>
[[nodiscard]] U dot(const point_t<U>& lhs, const point_t<U>& rhs) {
  return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

// cross product of 2 vectors
template<typename U>
[[nodiscard]] point_t<U> cross(const point_t<U>& lhs, const point_t<U>& rhs) {
  /*
  cross product equal to determinant of this matrix:
  | x (e1) | y (e2) | z (e3) |
  | x1     | y1     | z1     |
  | x2     | y2     | z2     |
  e1, e2, e3 - basis vectors
  */
  point_t res{
      lhs.y * rhs.z - rhs.y * lhs.z,
    -(lhs.x * rhs.z - rhs.x * lhs.z),
      lhs.x * rhs.y - rhs.x * lhs.y
  };

  return res;
}

template<typename U>
[[nodiscard]] U mixed_prod(
  const vector_t<U>& a,
  const vector_t<U>& b,
  const vector_t<U>& c
) {
  return dot(a, cross(b, c));
}

};

template<typename U>
inline point_t<U> point_t<U>::operator*(U coeff) const {
  return point_t<U>(x * coeff, y * coeff, z * coeff);
}

template<typename U>
[[nodiscard]] inline bool point_t<U>::operator==(const vector_t<U>& other) const {
  return utils::sign(x - other.x) == utils::signs_t::ZERO &&
         utils::sign(y - other.y) == utils::signs_t::ZERO &&
         utils::sign(z - other.z) == utils::signs_t::ZERO;
}

template<typename U>
std::istream& operator>>(std::istream& in_stream, point_t<U>& point) {
  auto& [x, y, z] = point;
  in_stream >> x >> y >> z;
  return in_stream;
}

template<typename U>
std::ostream& operator<<(std::ostream& out_stream, const point_t<U>& point) {
  auto [x, y, z] = point;
  out_stream << "(" << x << ", " << y << ", " << z << ")";
  return out_stream;
}
