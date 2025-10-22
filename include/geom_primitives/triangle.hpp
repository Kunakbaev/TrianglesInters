#pragma once

#include <array>
#include <iostream>
#include <vector>

#include "plane.hpp"
#include "point.hpp"
#include "AABB.hpp"

template<typename T>
class triangle_t {
 public:
  triangle_t() = default;

  triangle_t(const point_t<T>& a, const point_t<T>& b, const point_t<T>& c);

  [[nodiscard]] bool does_intersect(const triangle_t& other) const;

  [[nodiscard]] AABB_t<T> get_AABB() const;

  [[nodiscard]] point_t<T> get_center() const;

  [[nodiscard]] point_t<T> get_centroid() const;

  [[nodiscard]] std::array<point_t<T>, 3> get_points() const;

  [[nodiscard]] bool is_point_inside_triang(const point_t<T>& point) const;

  [[nodiscard]] bool is_intersected_by_segm(const segment_t<T>& segm) const;

 private:
  [[nodiscard]] std::array<segment_t<T>, 3> get_segments() const;

  // return 1, if B is A, rotated counterclockwise
  // return -1 if rotation is clockwise, 0 if they are collinear
  [[nodiscard]] utils::signs_t rotation_sign(
    const point_t<T>& p, const point_t<T>& a, const point_t<T>& b) const;

  [[nodiscard]] bool does_intersect_helper(const triangle_t& other) const;

 private:
  point_t<T> a_;
  point_t<T> b_;
  point_t<T> c_;
  AABB_t<T>  bounding_box_;
  point_t<T> center_;
  
  // ASK: it's convenient to use those, but takes more memory
  bool         is_triang_segm_{};
  plane_t<T>   plane_;
  segment_t<T> deg_case_segm_;
};

template<typename T>
triangle_t<T>::triangle_t(const point_t<T>& a, const point_t<T>& b, const point_t<T>& c)
    : a_(a), b_(b), c_(c), bounding_box_(*this),
      center_((bounding_box_.get_max_corner() + bounding_box_.get_min_corner()) * static_cast<T>(0.5)),
      is_triang_segm_(false), deg_case_segm_(a, b) {

  // constructing plane_ from points may result in exception
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

template<typename U>
[[nodiscard]] inline std::array<segment_t<U>, 3> triangle_t<U>::get_segments() const {
  return {
    segment_t<U>{a_, b_},
    segment_t<U>{b_, c_},
    segment_t<U>{c_, a_}
  };
}

// return 1, if PB is PA, rotated counterclockwise
// return -1 if rotation is clockwise, 0 if they are collinear
template<typename U>
[[nodiscard]] inline utils::signs_t triangle_t<U>::rotation_sign(
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

  utils::signs_t sign1 = rotation_sign(point, a_, b_);
  utils::signs_t sign2 = rotation_sign(point, b_, c_);
  utils::signs_t sign3 = rotation_sign(point, c_, a_);

  bool all_non_neg = sign1 >= utils::signs_t::ZERO &&
                     sign2 >= utils::signs_t::ZERO &&
                     sign3 >= utils::signs_t::ZERO;
  bool all_non_pos = sign1 <= utils::signs_t::ZERO &&
                     sign2 <= utils::signs_t::ZERO &&
                     sign3 <= utils::signs_t::ZERO;
  return all_non_neg || all_non_pos;
}

template<typename U>
inline bool triangle_t<U>::is_intersected_by_segm(const segment_t<U>& segm) const {
  if (is_triang_segm_) {
    return deg_case_segm_.does_inter(segm);
  }

  if (plane_.is_segment_on_plane(segm)) {
    std::array<segment_t<U>, 3> triang_segms = get_segments();
    for (const auto& triang_segm : triang_segms) {
      if (triang_segm.does_inter(segm)) {
        return true;
      }
    }

    return is_point_inside_triang(segm.get_start()) ||
           is_point_inside_triang(segm.get_finish());
  }

  auto [inter, is_inter] = plane_.intersect_by_segm(segm);
  if (!is_inter) {
    return false;
  }

  return is_point_inside_triang(inter);
}

template<typename U>
inline bool triangle_t<U>::does_intersect_helper(const triangle_t<U>& other) const {
  std::array<segment_t<U>, 3> segms = other.get_segments();
  for (const auto& segm : segms) {
    if (is_intersected_by_segm(segm)) {
      return true;
    }
  }

  return false;
}

template<typename U>
inline bool triangle_t<U>::does_intersect(const triangle_t<U>& other) const {
  return does_intersect_helper(other) ||
   other.does_intersect_helper(*this);
}

template <typename U>
[[nodiscard]] AABB_t<U> triangle_t<U>::get_AABB() const {
  return bounding_box_;
}

template <typename U>
[[nodiscard]] point_t<U> triangle_t<U>::get_center() const {
  return center_;
}

template <typename U>
[[nodiscard]] point_t<U> triangle_t<U>::get_centroid() const {
  return (a_ + b_ + c_) * static_cast<U>(0.5);
}

template <typename U>
[[nodiscard]] std::array<point_t<U>, 3> triangle_t<U>::get_points() const {
  return {a_, b_, c_};
}

template<typename U>
std::istream& operator>>(std::istream& in_stream, triangle_t<U>& triangle) {
  point_t<U> a;
  point_t<U> b;
  point_t<U> c;
  in_stream >> a >> b >> c;
  triangle = triangle_t<U>{a, b, c};

  return in_stream;
}

template<typename U>
std::ostream& operator<<(std::ostream& out_stream, const triangle_t<U>& triangle) {
  std::array<point_t<U>, 3> points = triangle.get_points();
  out_stream << "{" << points[0] << ", " << points[1] << ", " << points[2] << "}";
  return out_stream;
}
