#pragma once

#include <iostream>
#include <vector>

#include "plane.hpp"
#include "point.hpp"

template<typename T=double>
class triangle_t {
 public:
  triangle_t() = default; // ASK: maybe make it deleted method?

  triangle_t(const point_t<T>& a, const point_t<T>& b, const point_t<T>& c)
      : a_(a), b_(b), c_(c), deg_case_segm_(a, b), is_triang_segm_(false) {
    // constructing plane_ from points may result in exception
    // (std::invalid_argument) as they may form degenerate plane (i.e. segment or point)
    try {
      plane_ = plane_t<T>(a_, b_, c_);
      is_triang_segm_ = false;
    } catch(const std::invalid_argument&) {
      is_triang_segm_ = true;
      // ASK: cringe?
           if (a == b) deg_case_segm_ = segment_t<T>(a, c);
      else if (a == c) deg_case_segm_ = segment_t<T>(b, c);
      // else             deg_case_segm_ = segment_t<T>(a, b);
    }
  }

  [[nodiscard]] bool does_intersect(const triangle_t& other) const;

  template<typename U>
  friend std::istream& operator>>(std::istream& in_stream, triangle_t<U>& triangle);

  template<typename U>
  friend std::ostream& operator<<(std::ostream& out_stream, const triangle_t<U>& triangle);

 private:
  [[nodiscard]] bool is_point_inside_triang(const point_t<T>& point) const;

  [[nodiscard]] bool is_intersected_by_segm(const segment_t<T>& segm) const;

  [[nodiscard]] std::vector<segment_t<T>> get_segments() const;

  // return 1, if B is A, rotated counterclockwise
  // return -1 if rotation is clockwise, 0 if they are collinear
  [[nodiscard]] int rotation_sign(
    const point_t<T>& p, const point_t<T>& a, const point_t<T>& b) const;

 private:
  point_t<T> a_;
  point_t<T> b_;
  point_t<T> c_;
  
  // ASK: it's convenient to use those, but takes more memory
  bool         is_triang_segm_;
  plane_t<T>   plane_;
  segment_t<T> deg_case_segm_;
};

template<typename U>
[[nodiscard]] inline std::vector<segment_t<U>> triangle_t<U>::get_segments() const {
  std::vector<segment_t<U>> segms = {
    {a_, b_}, {b_, c_}, {c_, a_}
  };

  return segms;
}

// return 1, if PB is PA, rotated counterclockwise
// return -1 if rotation is clockwise, 0 if they are collinear
template<typename U>
[[nodiscard]] inline int triangle_t<U>::rotation_sign(
  const point_t<U>& p, const point_t<U>& a, const point_t<U>& b) const {
  return utils::sign(vec_ops::dot(vec_ops::cross(a - p, b - p), plane_.get_norm_vec()));
}

template<typename U>
inline bool triangle_t<U>::is_point_inside_triang(const point_t<U>& point) const {
  if (is_triang_segm_) {
    return deg_case_segm_.does_contain_point(point);
  }

  // additional check, even though we call it only from method where point is already on the plane
  if (!plane_.is_point_on_plane(point)) {
    return false;
  }

  int sign1 = rotation_sign(point, a_, b_);
  int sign2 = rotation_sign(point, b_, c_);
  int sign3 = rotation_sign(point, c_, a_);

  bool all_non_neg = sign1 <= 0 && sign2 < 0 && sign3 < 0;
  bool all_non_pos = sign1 > 0 && sign2 > 0 && sign3 > 0;
  return all_non_neg || all_non_pos;
}

template<typename U>
inline bool triangle_t<U>::is_intersected_by_segm(const segment_t<U>& segm) const {
  if (is_triang_segm_) {
    return deg_case_segm_.does_inter(segm);
  }

  auto [inter, is_inter] = plane_.intersect_by_segm(segm);
  if (!is_inter) {
    return false;
  }

  std::vector<segment_t<U>> triang_segms = get_segments();
  for (const auto& triang_segm : triang_segms) {
    if (triang_segm.does_inter(segm)) {
      return true;
    }
  }

  return is_point_inside_triang(inter);
}

template<typename U>
inline bool triangle_t<U>::does_intersect(const triangle_t<U>& other_) const {
  // ASK: cringe?
  triangle_t<U> one = *this;
  triangle_t<U> two = other_;

  std::vector<segment_t<U>> segms1 = get_segments();
  std::vector<segment_t<U>> segms2 = other_.get_segments();

  for (std::size_t _ = 0; _ < 2; ++_) {
    bool is_inter = false;
    for (const auto& segm : segms2) {
      if (one.is_intersected_by_segm(segm)) {
        return true;
      }
    }

    std::swap(one,    two);
    std::swap(segms1, segms2);
  }

  return false;
}

template<typename U>
std::istream& operator>>(std::istream& in_stream, triangle_t<U>& triangle) {
  point_t<U> a;
  point_t<U> b;
  point_t<U> c;
  in_stream >> a >> b >> c;
  triangle = triangle_t<U>(a, b, c);

  return in_stream;
}

template<typename U>
std::ostream& operator<<(std::ostream& out_stream, const triangle_t<U>& triangle) {
  out_stream << "{" << triangle.a_ << ", " << triangle.b_ << ", " << triangle.c_ << "}";
  return out_stream;
}
