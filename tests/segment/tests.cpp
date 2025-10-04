#include "logLib.hpp"

#include <gtest/gtest.h>
#include "point.hpp"
#include "segment.hpp"

TEST(SegmentTest, IsEmpty) {
  point_t<double> p1{1.0, 2.0, 3.0};
  point_t<double> p2{1.0, 2.0, 3.0};
  point_t<double> p3{1.5, 2.5, 3.5};
  
  segment_t seg1{p1, p2};
  segment_t seg2{p1, p3};
  
  EXPECT_TRUE(seg1.is_empty());
  EXPECT_FALSE(seg2.is_empty());
}

TEST(SegmentTest, DoesContainPoint) {
  point_t<double> start{0.0, 0.0, 0.0};
  point_t<double> end{2.0, 2.0, 2.0};
  point_t<double> middle{1.0, 1.0, 1.0};
  point_t<double> outside{3.0, 3.0, 3.0};
  point_t<double> not_on_line{1.0, 2.0, 1.0};
  
  segment_t seg{start, end};
  
  EXPECT_TRUE(seg.does_contain_point(start));
  EXPECT_TRUE(seg.does_contain_point(end));
  EXPECT_TRUE(seg.does_contain_point(middle));
  EXPECT_FALSE(seg.does_contain_point(outside));
  EXPECT_FALSE(seg.does_contain_point(not_on_line));
}

TEST(SegmentTest, GetDir) {
  point_t<double> p1{1.234, 2.567, 3.891};
  point_t<double> p2{4.321, 5.432, 6.543};
  point_t<double> expected_dir{3.087, 2.865, 2.652};
  
  segment_t seg{p1, p2};
  point_t<double> dir = seg.get_dir();
  
  EXPECT_TRUE(dir == expected_dir);
}

TEST(SegmentTest, GetStart) {
  point_t<double> start{1.234, 5.678, 9.012};
  point_t<double> end{3.456, 7.890, 2.468};
  
  segment_t seg{start, end};
  point_t<double> retrieved_start = seg.get_start();
  
  EXPECT_TRUE(retrieved_start == start);
}

TEST(SegmentTest, DoesIntersectCollinear) {
  point_t<double> a1{0.0, 0.0, 0.0};
  point_t<double> a2{3.0, 3.0, 3.0};
  point_t<double> b1{1.0, 1.0, 1.0};
  point_t<double> b2{2.0, 2.0, 2.0};
  point_t<double> b3{4.0, 4.0, 4.0};
  point_t<double> b4{5.0, 5.0, 5.0};
  
  segment_t seg1{a1, a2};
  segment_t seg2{b1, b2};  // fully inside
  segment_t seg3{b3, b4};  // no overlap
  
  EXPECT_TRUE(seg1.does_inter(seg2));
  EXPECT_FALSE(seg1.does_inter(seg3));
}

TEST(SegmentTest, DoesIntersectPerpendicular) {
  point_t<double> a1{0.0, 0.0, 0.0};
  point_t<double> a2{2.0, 0.0, 0.0};
  point_t<double> b1{1.0, -1.0, 0.0};
  point_t<double> b2{1.0, 1.0, 0.0};
  point_t<double> b3{3.0, -1.0, 0.0};
  point_t<double> b4{3.0, 1.0, 0.0};
  
  segment_t horizontal{a1, a2};
  segment_t vertical{b1, b2};    // intersects at (1,0,0)
  segment_t vertical_far{b3, b4}; // no intersection
  
  EXPECT_TRUE(horizontal.does_inter(vertical));
  EXPECT_FALSE(horizontal.does_inter(vertical_far));
}

TEST(SegmentTest, DoesIntersect3D) {
  point_t a1{1.0, 1.0, 1.0};
  point_t a2{3.0, 3.0, 3.0};
  point_t b1{1.0, 3.0, 1.0};
  point_t b2{3.0, 1.0, 3.0};
  
  segment_t seg1{a1, a2};  // diagonal in 3D
  segment_t seg2{b1, b2};  // crossing diagonal that intersects at (2,2,2)
  
  EXPECT_TRUE(seg1.does_inter(seg2));
}

TEST(SegmentTest, DoesIntersectSamePoint) {
  point_t<double> p1{1.5, 2.5, 3.5};
  point_t<double> p2{1.5, 2.5, 3.5};
  point_t<double> p3{4.5, 5.5, 6.5};
  
  segment_t seg1{p1, p2};  // point segment
  segment_t seg2{p1, p3};  // normal segment
  segment_t seg3{p1, p1};  // another point segment
  
  EXPECT_TRUE(seg1.does_inter(seg2));  // point segment contains shared point
  EXPECT_TRUE(seg1.does_inter(seg3));  // both are points at same location
}

TEST(SegmentTest, DoesIntersectParallel) {
  point_t<double> a1{0.0, 0.0, 0.0};
  point_t<double> a2{2.0, 0.0, 0.0};
  point_t<double> b1{0.0, 1.0, 0.0};
  point_t<double> b2{2.0, 1.0, 0.0};
  point_t<double> c1{0.5, 0.0, 0.0};
  point_t<double> c2{1.5, 0.0, 0.0};
  
  segment_t seg1{a1, a2};
  segment_t seg2{b1, b2};  // parallel, different lines
  segment_t seg3{c1, c2};  // collinear, overlapping
  
  EXPECT_FALSE(seg1.does_inter(seg2));
  EXPECT_TRUE(seg1.does_inter(seg3));
}

TEST(SegmentTest, DoesIntersectSkewLines) {
  point_t<double> a1{0.0, 0.0, 0.0};
  point_t<double> a2{2.0, 2.0, 0.0};
  point_t<double> b1{1.0, 0.0, 1.0};
  point_t<double> b2{1.0, 2.0, -1.0};
  
  segment_t seg1{a1, a2};  // in XY plane
  segment_t seg2{b1, b2};  // crosses through seg1's plane
  
  EXPECT_TRUE(seg1.does_inter(seg2));
}

TEST(SegmentTest, DoesIntersectByEndPoints) {
  point_t<double> a1{0.0, 0.0, 0.0};
  point_t<double> a2{2.0, 0.0, 0.0};
  point_t<double> b1{1.0, 0.0, 0.0};
  point_t<double> b2{1.0, 2.0, 0.0};
  
  segment_t seg1{a1, a2};
  segment_t seg2{b1, b2};
  
  EXPECT_TRUE(seg1.does_inter(seg2));
}

TEST(SegmentTest, DoesIntersectOnSameLine) {
  point_t<double> a1{0.0, 0.0, 0.0};
  point_t<double> a2{2.0, 0.0, 0.0};
  point_t<double> b1{-1.0, 0.0, 0.0};
  point_t<double> b2{1.0, 0.0, 0.0};
  
  segment_t seg1{a1, a2};
  segment_t seg2{b1, b2};
  
  EXPECT_TRUE(seg1.does_inter(seg2));
}

TEST(SegmentTest, DoesIntersectOnSameLine_2dots) {
  point_t<double> a1{2.0, 0.0, 0.0};
  point_t<double> a2{2.0, 0.0, 0.0};
  point_t<double> b1{2.0, 0.0, 0.0};
  point_t<double> b2{2.0, 0.0, 0.0};
  
  segment_t seg1{a1, a2};
  segment_t seg2{b1, b2};
  
  EXPECT_TRUE(seg1.does_inter(seg2));
}

TEST(SegmentTest, DoesNotIntersect3D) {
  point_t<double> a1{1.234, 2.345, 3.456};
  point_t<double> a2{2.567, 3.678, 4.789};
  point_t<double> b1{5.0, 6.0, 7.0};
  point_t<double> b2{6.0, 7.0, 8.0};
  
  segment_t seg1{a1, a2};
  segment_t seg2{b1, b2};  // far away segments
  
  EXPECT_FALSE(seg1.does_inter(seg2));
}

TEST(SegmentTest, DoesNotIntersect3D_2) {
  point_t<double> a1{1.234, 2.345, 0.0};
  point_t<double> a2{4.567, 5.678, 0.0};
  point_t<double> b1{3.0, 1.5, -1.0};
  point_t<double> b2{3.0, 6.5, 1.0};
  
  segment_t seg1{a1, a2};  // segment in XY plane
  segment_t seg2{b1, b2};  // seg2 is close to intersecting seg1
  
  EXPECT_FALSE(seg1.does_inter(seg2));
}

TEST(SegmentTest, DoesNotIntersect2dots) {
  point_t<double> a1{2.0, 0.0, 0.0};
  point_t<double> a2{2.0, 0.0, 0.0};
  point_t<double> b1{-2.0, 0.0, 0.0};
  point_t<double> b2{-2.0, 0.0, 0.0};
  
  segment_t seg1{a1, a2};  // in XY plane
  segment_t seg2{b1, b2};  // crosses through seg1's plane
  
  EXPECT_FALSE(seg1.does_inter(seg2));
}




// ----------- MORE TESTS WITH DEGENERATE CASES (when segment becomes point)

TEST(SegmentTest, PointSegmentIntersection) {
  point_t point_seg_start{1.234, 2.567, 3.891};
  point_t point_seg_end{1.234, 2.567, 3.891};
  point_t normal_seg_start{0.123, 1.456, 2.789};
  point_t normal_seg_end{2.345, 3.678, 4.901};
  point_t far_point{5.0, 6.0, 7.0};
  
  segment_t point_segment{point_seg_start, point_seg_end};
  segment_t normal_segment{normal_seg_start, normal_seg_end};
  segment_t far_point_segment{far_point, far_point};
  
  // Point segment should intersect normal segment if point lies on it
  EXPECT_TRUE(point_segment.does_inter(point_segment)); // self-intersection
  EXPECT_FALSE(point_segment.does_inter(far_point_segment)); // different points
}

TEST(SegmentTest, PointSegmentOnNormalSegment) {
  point_t seg_start{0.0, 0.0, 0.0};
  point_t seg_end{2.0, 2.0, 2.0};
  point_t midpoint{1.0, 1.0, 1.0};
  
  segment_t normal_segment{seg_start, seg_end};
  segment_t point_at_mid{midpoint, midpoint};
  segment_t point_at_start{seg_start, seg_start};
  segment_t point_at_end{seg_end, seg_end};
  
  EXPECT_TRUE(point_at_mid.does_inter(normal_segment));
  EXPECT_TRUE(point_at_start.does_inter(normal_segment));
  EXPECT_TRUE(point_at_end.does_inter(normal_segment));
}

TEST(SegmentTest, PointSegmentOutsideNormalSegment) {
  point_t seg_start{1.111, 2.222, 3.333};
  point_t seg_end{2.222, 3.333, 4.444};
  point_t outside_point{0.0, 0.0, 0.0};
  point_t beyond_point{3.333, 4.444, 5.555};
  
  segment_t normal_segment{seg_start, seg_end};
  segment_t point_outside{outside_point, outside_point};
  segment_t point_beyond{beyond_point, beyond_point};
  
  EXPECT_FALSE(point_outside.does_inter(normal_segment));
  EXPECT_FALSE(point_beyond.does_inter(normal_segment));
}

TEST(SegmentTest, TwoPointSegments) {
  point_t point1{1.5, 2.5, 3.5};
  point_t point2{4.5, 5.5, 6.5};
  point_t point3{1.5, 2.5, 3.5}; // same as point1
  
  segment_t point_seg1{point1, point1};
  segment_t point_seg2{point2, point2};
  segment_t point_seg3{point3, point3};
  
  EXPECT_TRUE(point_seg1.does_inter(point_seg3)); // same point
  EXPECT_FALSE(point_seg1.does_inter(point_seg2)); // different points
}
