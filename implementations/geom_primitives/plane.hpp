#pragma once

#include <iostream>

#include "point.hpp"
#include "segment.hpp"

namespace err_msgs {
  const std::string degenerate_plane = "Error: plane must be valid, not a segment and not a point.";
};

/*

ASK:
I want this plane class to be robust to various input arguments
(degenerate cases when plane is a segment too)
But it seems that it's not plane's class responsibility
to check such cases, because methods start looking littered 

*/

template<typename T=double>
class plane_t {
 public:
  plane_t() = default; // is it really needed? I need in triangle realization

  plane_t(const point_t<T>& a, const point_t<T>& b, const point_t<T>& c)
      : base_(a), norm_(vec_ops::cross(b - a, c - a)) {
    if (!is_valid_plane()) {
      throw std::invalid_argument(err_msgs::degenerate_plane);
    }
  };

  static plane_t<T> construct_from_2vecs(const vector_t<T>& a, const vector_t<T>& vec1, const vector_t<T>& vec2) {
    plane_t<T> plane(a, a + vec1, a + vec2);
    return plane;
  }

  vector_t<T> get_norm_vec() const { return norm_; }

  [[nodiscard]] bool is_point_on_plane(const point_t<T>& point) const;

  [[nodiscard]] bool is_segment_on_plane(const segment_t<T>& segment) const;

  std::pair<point_t<T>, bool> intersect_by_segm(
    const segment_t<T>& segm
  ) const;

  // checks if plane is valid, i.e. not a segment and not a point
  [[nodiscard]] bool is_valid_plane() const { return !norm_.is_zero(); }
  
  [[nodiscard]] point_t<T> get_point_on_plane() const { return base_; }

 private:
  point_t<T> base_; // any point on the plane
  point_t<T> norm_; // if == 0, than plain is invalid -> segment or even point
};

template<typename U>
[[nodiscard]] bool plane_t<U>::is_point_on_plane(const point_t<U>& point) const {
  U dot_prod = vec_ops::dot(point - base_, norm_);
  return utils::sign(dot_prod) == utils::signs_t::ZERO;
}

template <typename U>
[[nodiscard]] bool plane_t<U>::is_segment_on_plane(const segment_t<U>& segment) const {
  return is_point_on_plane(segment.get_start()) ||
         is_point_on_plane(segment.get_finish());
}

template<typename U>
inline std::pair<point_t<U>, bool> plane_t<U>::intersect_by_segm(
  const segment_t<U>& segm
) const {
  vector_t<U> segm_dir = segm.get_dir();
  U numerator = vec_ops::dot(norm_, base_ - segm.get_start());
  U denom     = vec_ops::dot(norm_, segm_dir);
  if (utils::sign(denom) == utils::signs_t::ZERO) {
    // segment is parallel to triangle plane
    // so we check if both points are contained in the plane
    bool is_inter = is_point_on_plane(segm.get_start()) &&
                    is_point_on_plane(segm.get_finish());
    if (is_inter) {
      return {segm.get_start(), true};
    } else {
      return {{}, false};
    }
  }

  U segm_time = numerator / denom;
  if (utils::sign(    segm_time) == utils::signs_t::NEG ||
      utils::sign(1 - segm_time) == utils::signs_t::NEG) {
    // intersection point is not on the segment
    return {{}, false};
  }

  point_t<U> inter = segm.get_start() + (segm_dir * segm_time);
  return {inter, true};
}

template<typename U>
std::ostream& operator<<(std::ostream& out_stream, const plane_t<U>& plane) {
  const auto& [base, norm] = plane;
  out_stream << "{base: "   << plane.get_point_on_plane()
             << ", norm : " << plane.get_norm_vec() << "}";
  return out_stream;
}
