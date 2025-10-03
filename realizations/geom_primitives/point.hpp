#pragma once

#include <cassert>
#include <cmath>

#include "utils.hpp"

// template class, so it's easy to quickly change data type of coords without pain

template<typename T=double>
class point_t {
 public:
  point_t() = default;

  point_t(T x, T y, T z) : x_(x), y_(y), z_(z) {}

  [[nodiscard]] T get_len_sq() const;

  [[nodiscard]] T get_len() const;

  void norm();

  [[nodiscard]] bool is_zero() const;

  [[nodiscard]] point_t operator+(const point_t& other) const;
  [[nodiscard]] point_t operator-(const point_t& other) const;

  // dot product of 2 vectors
  template<typename U>
  [[nodiscard]] friend U dot(const point_t<U>& lhs, const point_t<U>& rhs);

  // cross product of 2 vectors
  template<typename U>
  [[nodiscard]] friend point_t<U> cross(const point_t<U>& lhs, const point_t<U>& rhs);

  point_t operator*(T coeff) const;

  template<typename U>
  friend std::istream& operator>>(std::istream& in_stream, point_t<U>& point);

  template<typename U>
  friend std::ostream& operator<<(std::ostream& out_stream, const point_t<U>& point);

 private:
  T x_;
  T y_;
  T z_;
};

template<typename U>
[[nodiscard]] inline U point_t<U>::get_len_sq() const {
  return utils::sqr(x_) + utils::sqr(y_) + utils::sqr(z_);
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
inline void point_t<U>::norm() {
  U len = get_len();
  if (utils::sign(len) != 0) {
    x_ /= len;
    y_ /= len;
    z_ /= len;
  }
}

template<typename U>
[[nodiscard]] bool point_t<U>::is_zero() const {
  // ASK: maybe this is better: return utils::sign(get_len_sq()) == 0;
  return utils::sign(x_) == 0 &&
         utils::sign(y_) == 0 &&
         utils::sign(z_) == 0;
}

template<typename U>
[[nodiscard]] inline point_t<U> point_t<U>::operator+(const point_t<U>& other) const {
  point_t<U> res(x_ + other.x_, y + other.y_, z_ + other.z_);
  return res;
}

template<typename U>
[[nodiscard]] inline point_t<U> point_t<U>::operator-(const point_t<U>& other) const {
  point_t<U> res(x_ - other.x_, y - other.y_, z_ - other.z_);
  return res;
}

namespace vec_ops {
// dot product of 2 vectors
template<typename U>
[[nodiscard]] U dot(const point_t<U>& lhs, const point_t<U>& rhs) {
  return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
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
      lhs.y_ * rhs.z_ - rhs.y_ * lhs.z_,
    -(lhs.x_ * rhs.z_ - rhs.x_ * lhs.z_),
      lhs.x_ * rhs.y_ - rhs.x_ * lhs.y_
  };

  return res;
}
};

template<typename U>
inline point_t<U> point_t<U>::operator*(U coeff) const {
  return point_t<U>(x_ * coeff, y_ * coeff, z_ * coeff);
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

// alias for vector as it's basically the same thing
template<class T>
using vector_t = point_t<T>; // TODO:
