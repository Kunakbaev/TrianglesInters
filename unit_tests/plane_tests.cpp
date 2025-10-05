#include <gtest/gtest.h>
#include "point.hpp"
#include "plane.hpp"
#include "segment.hpp"

TEST(PlaneTest, ConstructorFromThreePoints) {
  point_t<double> p1{1.234, 2.345, 3.456};
  point_t<double> p2{4.567, 5.678, 6.789};
  point_t<double> p3{7.890, 8.901, 9.012};
  
  plane_t plane{p1, p2, p3};

  EXPECT_TRUE(plane.is_valid_plane());
  EXPECT_TRUE(plane.is_point_on_plane(p1));
  EXPECT_TRUE(plane.is_point_on_plane(p2));
  EXPECT_TRUE(plane.is_point_on_plane(p3));
}

TEST(PlaneTest, ConstructorFromPointAndTwoVectors) {
  point_t<double> point{1.111, 2.222, 3.333};
  point_t<double> vec1{2.0, 0.0, 0.0};
  point_t<double> vec2{0.0, 3.0, 0.0};
  
  plane_t plane = plane_t<double>::construct_from_2vecs(point, vec1, vec2);
  
  EXPECT_TRUE(plane.is_valid_plane());
  EXPECT_TRUE(plane.is_point_on_plane(point));
  
  point_t<double> point_on_plane{3.111, 5.222, 3.333};
  EXPECT_TRUE(plane.is_point_on_plane(point_on_plane));
}

TEST(PlaneTest, IsPointOnPlane) {
  point_t<double> p1{0.0, 0.0, 0.0};
  point_t<double> p2{1.0, 0.0, 0.0};
  point_t<double> p3{0.0, 1.0, 0.0};
  
  plane_t plane{p1, p2, p3};
  
  point_t<double> point_on_plane{0.5, 0.5, 0.0};
  point_t<double> point_off_plane{0.5, 0.5, 1.234};
  
  EXPECT_TRUE(plane.is_point_on_plane(point_on_plane));
  EXPECT_FALSE(plane.is_point_on_plane(point_off_plane));
}

TEST(PlaneTest, GetNormVector) {
  point_t<double> p1{1.0, 2.0, 3.0};
  point_t<double> p2{4.0, 6.0, 8.0};
  point_t<double> p3{2.0, 3.0, 4.0};
  
  plane_t plane{p1, p2, p3};
  point_t<double> norm = plane.get_norm_vec();
  
  point_t<double> vec1 = p2 - p1;
  point_t<double> vec2 = p3 - p1;
  point_t<double> expected_norm = vec_ops::cross(vec1, vec2);
  
  // Check that norm is orthogonal to plane vectors
  EXPECT_NEAR(vec_ops::dot(norm, vec1), 0.0, 1e-9);
  EXPECT_NEAR(vec_ops::dot(norm, vec2), 0.0, 1e-9);
  
  // Check that norm is not zero
  double norm_length = vec_ops::dot(norm, norm);
  EXPECT_GT(norm_length, 1e-9);
}

TEST(PlaneTest, IntersectBySegment) {
  point_t<double> p1{0.0, 0.0, 0.0};
  point_t<double> p2{1.0, 0.0, 0.0};
  point_t<double> p3{0.0, 1.0, 0.0};
  
  plane_t plane{p1, p2, p3};
  
  // Segment intersecting the plane
  point_t<double> seg_start{0.5, 0.5, -1.0};
  point_t<double> seg_end{0.5, 0.5, 1.0};
  segment_t seg{seg_start, seg_end};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(seg);
  
  EXPECT_TRUE(intersects);
  EXPECT_TRUE(plane.is_point_on_plane(intersection_point));
}

TEST(PlaneTest, IntersectBySegmentParallel) {
  point_t<double> p1{0.0, 0.0, 0.0};
  point_t<double> p2{1.0, 0.0, 0.0};
  point_t<double> p3{0.0, 1.0, 0.0};
  
  plane_t plane{p1, p2, p3};
  
  // Segment parallel to plane (in plane z=1)
  point_t<double> seg_start{0.0, 0.0, 1.0};
  point_t<double> seg_end{1.0, 1.0, 1.0};
  segment_t seg{seg_start, seg_end};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(seg);
  
  EXPECT_FALSE(intersects);
}

TEST(PlaneTest, IntersectByPointSegmentOnPlane) {
  point_t p1{1.234, 2.567, 3.891};
  point_t p2{4.321, 5.432, 6.543};
  point_t p3{7.654, 8.765, 9.876};
  
  plane_t plane{p1, p2, p3};
  
  // Create a point that definitely lies on the plane
  point_t vec1 = p2 - p1;
  point_t vec2 = p3 - p1;
  point_t point_on_plane = p1 + vec1 * 0.3 + vec2 * 0.4;
  segment_t point_segment{point_on_plane, point_on_plane};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(point_segment);
  
  EXPECT_TRUE(intersects);
  EXPECT_TRUE(point_on_plane == intersection_point);
  EXPECT_TRUE(plane.is_point_on_plane(intersection_point));
}

TEST(PlaneTest, IntersectByPointSegmentOffPlane) {
  point_t<double> p1{0.0, 0.0, 0.0};
  point_t<double> p2{1.0, 0.0, 0.0};
  point_t<double> p3{0.0, 1.0, 0.0};
  
  plane_t plane{p1, p2, p3};
  
  // Point segment not on the plane
  point_t<double> point_off_plane{0.5, 0.5, 1.234};
  segment_t point_segment{point_off_plane, point_off_plane};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(point_segment);
  
  EXPECT_FALSE(intersects);
}

TEST(PlaneTest, ConstructorThrowsOnDegenerateCase) {
  point_t<double> p1{1.0, 2.0, 3.0};
  point_t<double> p2{1.0, 2.0, 3.0}; // same as p1
  point_t<double> p3{4.0, 5.0, 6.0};
  
  EXPECT_THROW({
    plane_t plane(p1, p2, p3);
  }, std::invalid_argument);
}

TEST(PlaneTest, ConstructFrom2VecsThrowsOnDegenerateCase) {
  point_t<double> point{1.111, 2.222, 3.333};
  point_t<double> vec1{2.0, 4.0, 6.0};
  point_t<double> vec2{1.0, 2.0, 3.0}; // parallel to vec1
  
  EXPECT_THROW({
    plane_t plane = plane_t<double>::construct_from_2vecs(point, vec1, vec2);
  }, std::invalid_argument);
}





TEST(PlaneTest, IntersectByPointSegmentContainedInPlane) {
  point_t p1{1.0, 2.0, 3.0};
  point_t p2{4.0, 6.0, 8.0};
  point_t p3{2.0, 3.0, 4.0};
  
  plane_t plane{p1, p2, p3};
  
  // Create a point that lies on the plane using linear combination
  point_t vec1 = p2 - p1;
  point_t vec2 = p3 - p1;
  point_t point_on_plane = p1 + vec1 * 0.3 + vec2 * 0.4;
  segment_t point_segment{point_on_plane, point_on_plane};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(point_segment);
  
  EXPECT_TRUE(intersects);
  EXPECT_TRUE(point_on_plane == intersection_point);
  EXPECT_TRUE(plane.is_point_on_plane(intersection_point));
}

TEST(PlaneTest, SegmentContainedInPlane) {
  point_t p1{0.0, 0.0, 0.0};
  point_t p2{1.0, 0.0, 0.0};
  point_t p3{0.0, 1.0, 0.0};
  
  plane_t plane{p1, p2, p3};
  
  // Entire segment lies in the plane
  point_t seg_start{0.2, 0.3, 0.0};
  point_t seg_end{0.7, 0.8, 0.0};
  segment_t seg{seg_start, seg_end};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(seg);
  
  EXPECT_TRUE(intersects);
  EXPECT_TRUE(plane.is_point_on_plane(intersection_point));
  // Should return one of the endpoints or the intersection point
}

TEST(PlaneTest, SegmentOneEndpointOnPlane) {
  point_t p1{1.234, 2.345, 3.456};
  point_t p2{4.567, 5.678, 6.789};
  point_t p3{7.890, 8.901, 9.012};
  
  plane_t plane{p1, p2, p3};
  
  // One endpoint on plane, other off plane
  point_t point_on_plane = p1; // use one of the plane points
  point_t point_off_plane = point_on_plane + plane.get_norm_vec(); // move along normal
  segment_t seg{point_on_plane, point_off_plane};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(seg);
  
  EXPECT_TRUE(intersects);
  EXPECT_TRUE(point_on_plane == intersection_point);
  EXPECT_TRUE(plane.is_point_on_plane(intersection_point));
}

TEST(PlaneTest, SegmentParallelToPlaneButNotContained) {
  point_t p1{0.0, 0.0, 0.0};
  point_t p2{1.0, 0.0, 0.0};
  point_t p3{0.0, 1.0, 0.0};
  
  plane_t plane{p1, p2, p3};
  
  // Segment parallel to plane but at z=1 (not in plane)
  point_t seg_start{0.1, 0.2, 1.0};
  point_t seg_end{0.8, 0.9, 1.0};
  segment_t seg{seg_start, seg_end};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(seg);
  
  EXPECT_FALSE(intersects);
}

TEST(PlaneTest, SegmentParallelToPlaneAndContained) {
  point_t p1{0.0, 0.0, 0.0};
  point_t p2{1.0, 0.0, 0.0};
  point_t p3{0.0, 1.0, 0.0};
  
  plane_t plane{p1, p2, p3};
  
  // Segment parallel to plane and contained in it
  point_t seg_start{0.3, 0.4, 0.0};
  point_t seg_end{0.6, 0.7, 0.0};
  segment_t seg{seg_start, seg_end};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(seg);
  
  EXPECT_TRUE(intersects);
  EXPECT_TRUE(plane.is_point_on_plane(intersection_point));
}

TEST(PlaneTest, PointSegmentNotOnPlane) {
  point_t p1{1.0, 1.0, 1.0};
  point_t p2{2.0, 3.0, 4.0};
  point_t p3{5.0, 6.0, 7.0};
  
  plane_t plane{p1, p2, p3};
  
  // Get plane normal and move point clearly off the plane
  point_t norm = plane.get_norm_vec();
  point_t point_off_plane = p1 + norm; // move along normal direction
  segment_t point_segment{point_off_plane, point_off_plane};
  
  auto [intersection_point, intersects] = plane.intersect_by_segm(point_segment);
  
  EXPECT_FALSE(intersects);
}
