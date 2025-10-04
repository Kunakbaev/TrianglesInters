#pragma once

#include "point.hpp"

template<typename T=double>
class segment_t {
 public:
  segment_t(const point_t<T>& a, const point_t<T>& b)
    : a_(a), b_(b) {}

  [[nodiscard]] bool is_empty() const;

  [[nodiscard]] bool does_contain_point(const point_t<T>& point) const;

  [[nodiscard]] point_t<T> get_dir() const;

  [[nodiscard]] point_t<T> get_start() const;

  [[nodiscard]] point_t<T> get_finish() const;

  [[nodiscard]] bool does_inter(const segment_t& other) const;

  template<typename U>
  friend std::ostream& operator<<(std::ostream& out_stream, const segment_t<U>& segm);

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
[[nodiscard]] point_t<U> segment_t<U>::get_finish() const {
  return b_;
}

template<typename U>
[[nodiscard]] inline bool segment_t<U>::does_contain_point(
  const point_t<U>& point
) const {
  if (is_empty()) {
    return (a_ - point).is_zero();
  }

  vector_t<U> start2point = point - get_start();
  vector_t<U> cross_prod = vec_ops::cross(get_dir(), start2point);
  if (!cross_prod.is_zero()) {
    return false;
  }

  U dot_prod = vec_ops::dot(get_dir(), start2point);
  // std::cerr << "dot_prod : " << dot_prod << " segm_len : " << get_dir().get_len_sq() << std::endl;
  if (utils::sign(dot_prod) < 0) return false;
  if (utils::sign(dot_prod - get_dir().get_len_sq()) > 0) return false;

  return true;

  // TODO: cringe
  // return does_lie_in_range(a_.x_, point.x_, b_.x_) &&
  //        does_lie_in_range(a_.y_, point.y_, b_.y_) &&
  //        does_lie_in_range(a_.z_, point.z_, b_.z_);
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
  U mix = vec_ops::mixed_prod(get_dir(), other.get_dir(), other.get_start() - get_start());
  // std::cout << "mix : " << mix << std::endl;
  if (utils::sign(mix) != 0) {
    return false;
  }

  if (check_endpoints_inters(other)) {
    return true;
  }

  // if one of segments is point
  if (is_empty() || other.is_empty()) {
    return false;
  }

  // a + t * (b - a) = c + s * (d - c);
  vector_t<U> v1 = get_dir();
  vector_t<U> v2 = other.get_dir();
  // std::cerr << "my : " << *this << " other : " << other << std::endl;
  // std::cerr << "v1 : " << v1 << " v2 : " << v2 << std::endl;
  vector_t<U> norm = vec_ops::cross(v1, v2);
  vector_t<U> starts_diff = get_start() - other.get_start();
  vector_t<U> cross_prod_numerator =
    vec_ops::cross(starts_diff, v2);
  U numerator = vec_ops::dot(cross_prod_numerator, norm);
  U denominator = norm.get_len_sq();
  // std::cerr <<  "denominatro : " << denominator << std::endl;
  if (utils::sign(denominator) == 0) {
    // both segments lie on the same line
    // we already checked that case with check_endpoints_inters method
    return false;
  }

  U segm_time = -numerator / denominator;
  // std::cerr << "segm_time : " << segm_time << std::endl;
  point_t<U> inter = get_start() + get_dir() * segm_time;
  // std::cerr << "inter : " << inter << std::endl;
  return       does_contain_point(inter) &&
         other.does_contain_point(inter);
}

template<typename U>
std::ostream& operator<<(std::ostream& out_stream, const segment_t<U>& segm) {
  out_stream << "{a: " << segm.a_ << ", b: " << segm.b_ << "}";
  return out_stream;
}
