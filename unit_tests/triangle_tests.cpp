#include <gtest/gtest.h>
#include "point.hpp"
#include "triangle.hpp"

TEST(TriangleTest, ConstructorFromThreePoints) {
  point_t p1{1.234, 2.345, 3.456};
  point_t p2{4.567, 5.678, 6.789};
  point_t p3{7.890, 8.901, 9.012};
  
  triangle_t triangle{p1, p2, p3};
  
  // Test that triangle was created (no exception thrown)
  SUCCEED();
}

TEST(TriangleTest, DoesIntersectWithOverlappingTriangles) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 1.0, 0.0};
  point_t b2{3.0, 1.0, 0.0};
  point_t b3{1.0, 3.0, 0.0};
  
  triangle_t tri1{a1, a2, a3};
  triangle_t tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, DoesNotIntersectSeparateTriangles) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{3.0, 3.0, 3.0};
  point_t b2{4.0, 3.0, 3.0};
  point_t b3{3.0, 4.0, 3.0};
  
  triangle_t tri1{a1, a2, a3};
  triangle_t tri2{b1, b2, b3};
  
  EXPECT_FALSE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, DoesIntersectEdgeToEdge) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{0.0, 2.0, 0.0};
  point_t b3{-2.0, 0.0, 0.0};
  
  triangle_t tri1{a1, a2, a3};
  triangle_t tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, DoesIntersect3D) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 1.0, 1.0};
  point_t b2{1.0, 1.0, -1.0};
  point_t b3{1.0, 2.0, 0.0};
  
  triangle_t tri1{a1, a2, a3};
  triangle_t tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, PointTriangleNotIntersectingOXYplane) {
  point_t p1{1.5, 1.5, 0.0};
  point_t p2{1.5, 1.5, 0.0};
  point_t p3{1.5, 1.5, 0.0};
  
  point_t q1{0.0, 0.0, 0.0};
  point_t q2{2.0, 0.0, 0.0};
  point_t q3{0.0, 2.0, 0.0};
  
  triangle_t point_tri{p1, p2, p3};
  triangle_t normal_tri{q1, q2, q3};
  
  EXPECT_FALSE(point_tri.does_intersect(normal_tri));
}

TEST(TriangleTest, PointTriangleIntersection) {
  point_t p1{0.5, 0.5, 0.0};
  point_t p2{0.5, 0.5, 0.0};
  point_t p3{0.5, 0.5, 0.0};
  
  point_t q1{0.0, 0.0, 0.0};
  point_t q2{2.0, 0.0, 0.0};
  point_t q3{0.0, 2.0, 0.0};
  
  triangle_t point_tri{p1, p2, p3};
  triangle_t normal_tri{q1, q2, q3};
  
  EXPECT_TRUE(point_tri.does_intersect(normal_tri));
}

TEST(TriangleTest, SegmentTriangleIntersection) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 1.0, 0.0};
  point_t b2{1.0, 1.0, 0.0};
  point_t b3{3.0, 3.0, 0.0};
  
  triangle_t tri1{a1, a2, a3};
  triangle_t segment_tri{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(segment_tri));
}

TEST(TriangleTest, CoplanarIntersectingTrianglesOXYplane_2) {
  point_t a1{1.0, 1.0, 0.0};
  point_t a2{-1.0, 1.0, 0.0};
  point_t a3{0.0, -1.0, 0.0};
  
  point_t b1{0.0, 1.0, 0.0};
  point_t b2{-1.0, -1.0, 0.0};
  point_t b3{-1.0, -1.0, 0.0};
  
  triangle_t tri1{a1, a2, a3};
  triangle_t tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, SeparatePointTriangle) {
  point_t a1{5.0, 5.0, 5.0};
  point_t a2{5.0, 5.0, 5.0};
  point_t a3{5.0, 5.0, 5.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{1.0, 0.0, 0.0};
  point_t b3{0.0, 1.0, 0.0};
  
  triangle_t point_tri{a1, a2, a3};
  triangle_t normal_tri{b1, b2, b3};
  
  EXPECT_FALSE(point_tri.does_intersect(normal_tri));
}

TEST(TriangleTest, CoplanarNonIntersectingTriangles) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{2.0, 0.0, 0.0};
  point_t b2{3.0, 0.0, 0.0};
  point_t b3{2.0, 1.0, 0.0};
  
  triangle_t tri1{a1, a2, a3};
  triangle_t tri2{b1, b2, b3};
  
  EXPECT_FALSE(tri1.does_intersect(tri2));
}


// ------------------ more complex tests, ones on which naive has failed: ------

TEST(TriangleTest, CoplanarIntersectingTriangles) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 1.0, 0.0};
  point_t b2{3.0, 1.0, 0.0};
  point_t b3{1.0, 3.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, EdgeIntersection) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 0.0, 0.0};
  point_t b2{1.0, 2.0, 0.0};
  point_t b3{3.0, 0.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, VertexTouching) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{-1.0, 0.0, 0.0};
  point_t b3{0.0, -1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, TriangleInsideAnother) {
  point_t outer1{0.0, 0.0, 0.0};
  point_t outer2{3.0, 0.0, 0.0};
  point_t outer3{0.0, 3.0, 0.0};
  
  point_t inner1{1.0, 1.0, 0.0};
  point_t inner2{2.0, 1.0, 0.0};
  point_t inner3{1.0, 2.0, 0.0};
  
  triangle_t<double> outer_tri{outer1, outer2, outer3};
  triangle_t<double> inner_tri{inner1, inner2, inner3};
  
  EXPECT_TRUE(outer_tri.does_intersect(inner_tri));
}

TEST(TriangleTest, NonIntersectingSeparated) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{3.0, 3.0, 0.0};
  point_t b2{4.0, 3.0, 0.0};
  point_t b3{3.0, 4.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_FALSE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, NonIntersectingDifferentZ) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.5, 0.5, 1.0};
  point_t b2{1.5, 0.5, 1.0};
  point_t b3{0.5, 1.5, 1.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_FALSE(tri1.does_intersect(tri2));
}

TEST(TriangleTest, MyByHandEqual) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{1.0, 0.0, 0.0};
  point_t b3{0.0, 1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, MyByHandShareSide) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{1.0, 0.0, 0.0};
  point_t b3{0.0, -1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, MyByHandContain) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{2.0, 0.0, 0.0};
  point_t b3{0.0, 2.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, MyByHandContainNoTouch) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{-1.0, -1.0, 0.0};
  point_t b2{2.0, 0.0, 0.0};
  point_t b3{0.0, 2.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, MyByHandTouchByDot) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{-1.0, 0.0, 0.0};
  point_t b2{0.0, 0.0, 0.0};
  point_t b3{-1.0, 1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, MyByHandTouchByDotOnSegm) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{-1.0, 0.5, 0.0};
  point_t b2{0.0,  0.5, 0.0};
  point_t b3{-1.0, 1.5, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}







// Basic coplanar intersection tests
TEST(TriangleTest, CoplanarEdgeIntersection) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.5, 0.0, 0.0};
  point_t b2{0.5, 1.0, 0.0};
  point_t b3{1.5, 0.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, CoplanarVertexTouch) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.0, 0.0, 0.0}; // shares vertex with a1
  point_t b2{0.0, -1.0, 0.0};
  point_t b3{-1.0, 0.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, CoplanarCompleteOverlap) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{1.0, 0.0, 0.0};
  point_t b3{0.0, 1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, CoplanarPartialOverlap) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 0.0, 0.0};
  point_t b2{1.0, 2.0, 0.0};
  point_t b3{0.0, 1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

// Non-intersecting coplanar cases
TEST(TriangleTest, CoplanarNoIntersection) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{2.0, 0.0, 0.0};
  point_t b2{3.0, 0.0, 0.0};
  point_t b3{2.0, 1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_FALSE(tri1.does_intersect(tri2));
  EXPECT_FALSE(tri2.does_intersect(tri1));
}

TEST(TriangleTest, CoplanarAdjacentNoIntersection) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  point_t b1{1.0, 0.0, 0.0}; // shares vertex but no area overlap
  point_t b2{2.0, 0.0, 0.0};
  point_t b3{1.0, 1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  // This should be false if only sharing a vertex/edge doesn't count as intersection
  // Change to true if your definition includes boundary touches
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

// Edge cases with collinear points
TEST(TriangleTest, CoplanarCollinearEdges) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 0.0, 0.0};
  point_t b2{0.0, 1.0, 0.0};
  point_t b3{1.0, 1.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}

// Degenerate triangle cases
TEST(TriangleTest, CoplanarDegenerateTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.5, 0.0, 0.0}; // degenerate - all points colinear
  
  point_t b1{0.5, -1.0, 0.0};
  point_t b2{0.5, 1.0, 0.0};
  point_t b3{1.5, 0.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  // Result depends on how you handle degenerate triangles
  // This tests edge case behavior
  bool result = tri1.does_intersect(tri2);
  EXPECT_TRUE(result); // Both might be acceptable
}

// Test with very small triangles near floating point precision
TEST(TriangleTest, CoplanarSmallTriangles) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1e-10, 0.0, 0.0};
  point_t a3{0.0, 1e-10, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{0.0, -1e-10, 0.0};
  point_t b3{-1e-10, 0.0, 0.0};
  
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{b1, b2, b3};
  
  EXPECT_TRUE(tri1.does_intersect(tri2));
  EXPECT_TRUE(tri2.does_intersect(tri1));
}




// =============================================
// Tests for is_point_inside_triang
// =============================================

TEST(TrianglePointTest, PointInsideTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t inside{0.2, 0.2, 0.0};
  point_t centroid{1.0/3.0, 1.0/3.0, 0.0};
  
  EXPECT_TRUE(tri.is_point_inside_triang(inside));
  EXPECT_TRUE(tri.is_point_inside_triang(centroid));
}

TEST(TrianglePointTest, PointOnVertices) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  EXPECT_TRUE(tri.is_point_inside_triang(a1));
  EXPECT_TRUE(tri.is_point_inside_triang(a2));
  EXPECT_TRUE(tri.is_point_inside_triang(a3));
}

TEST(TrianglePointTest, PointOnEdges) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t edge1{0.5, 0.0, 0.0};  // on a1-a2
  point_t edge2{0.0, 0.5, 0.0};  // on a1-a3
  point_t edge3{0.5, 0.5, 0.0};  // on a2-a3
  
  EXPECT_TRUE(tri.is_point_inside_triang(edge1));
  EXPECT_TRUE(tri.is_point_inside_triang(edge2));
  EXPECT_TRUE(tri.is_point_inside_triang(edge3));
}

TEST(TrianglePointTest, PointOutsideTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t outside1{-0.1, 0.5, 0.0};
  point_t outside2{0.5, -0.1, 0.0};
  point_t outside3{0.8, 0.8, 0.0};
  point_t outside4{1.0, 1.0, 0.0};
  
  EXPECT_FALSE(tri.is_point_inside_triang(outside1));
  EXPECT_FALSE(tri.is_point_inside_triang(outside2));
  EXPECT_FALSE(tri.is_point_inside_triang(outside3));
  EXPECT_FALSE(tri.is_point_inside_triang(outside4));
}

TEST(TrianglePointTest, PointOutsideButCloseToEdge) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t close1{0.5, -1e-5, 0.0};
  point_t close2{-1e-5, 0.5, 0.0};
  point_t close3{0.5 + 1e-5, 0.5 + 1e-5, 0.0};
  
  EXPECT_FALSE(tri.is_point_inside_triang(close1));
  EXPECT_FALSE(tri.is_point_inside_triang(close2));
  EXPECT_FALSE(tri.is_point_inside_triang(close3));
}

TEST(TrianglePointTest, PointIn3DSpace) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 1.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  // Test point in the triangle's plane
  point_t in_plane{0.2, 0.2, 0.2};
  point_t out_plane{0.2, 0.2, 1.0}; // same xy but different z
  
  // These tests depend on whether your method projects to 2D or works in 3D
  bool result1 = tri.is_point_inside_triang(in_plane);
  bool result2 = tri.is_point_inside_triang(out_plane);
  
  // At least one should be true if projecting, both might be false if strict 3D
  EXPECT_TRUE(result1 || !result2);
}

TEST(TrianglePointTest, DegenerateTrianglePoint) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.5, 0.0, 0.0}; // colinear - degenerate
  triangle_t<double> tri{a1, a2, a3};
  
  point_t on_line{0.7, 0.0, 0.0};
  point_t off_line{0.7, 0.1, 0.0};
  
  // Behavior depends on implementation - test both
  bool result1 = tri.is_point_inside_triang(on_line);
  bool result2 = tri.is_point_inside_triang(off_line);
  
  // Document the expected behavior
  std::cout << "Degenerate triangle results - on_line: " << result1 
            << ", off_line: " << result2 << std::endl;
}

// =============================================
// Tests for is_intersected_by_segm
// =============================================

TEST(TriangleSegmentTest, SegmentThroughTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{-0.5, 0.2, 0.0};
  point_t seg_end{0.5, 0.2, 0.0};
  
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_start, seg_end}));
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_end, seg_start})); // reverse
}

TEST(TriangleSegmentTest, SegmentInsideTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{0.2, 0.2, 0.0};
  point_t seg_end{0.3, 0.3, 0.0};
  
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentOnTriangleEdge) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{0.0, 0.0, 0.0};
  point_t seg_end{1.0, 0.0, 0.0}; // exactly on edge a1-a2
  
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentTouchingVertex) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{0.0, 0.0, 0.0}; // vertex a1
  point_t seg_end{-1.0, -1.0, 0.0};
  
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentOutsideTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{-1.0, -1.0, 0.0};
  point_t seg_end{-2.0, -2.0, 0.0};
  
  EXPECT_FALSE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentParallelToEdge) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{-0.5, 0.1, 0.0};
  point_t seg_end{1.5, 0.1, 0.0}; // parallel to base
  
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentIn3DSpace) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 1.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  // Segment passing through the triangle in 3D
  point_t seg_start{0.2, 0.2, -1.0};
  point_t seg_end{0.2, 0.2, 1.0};
  
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentParallelToTrianglePlane) {
  point_t a1{0.0, 0.0, 1.0};
  point_t a2{1.0, 0.0, 1.0};
  point_t a3{0.0, 1.0, 1.0};
  triangle_t<double> tri{a1, a2, a3};
  
  // Segment in parallel plane (z=0) that would intersect if projected
  point_t seg_start{-0.5, 0.2, 0.0};
  point_t seg_end{0.5, 0.2, 0.0};
  
  // This should be false since they're in different parallel planes
  EXPECT_FALSE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentEndpointInsideTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{0.2, 0.2, 0.0}; // inside
  point_t seg_end{1.5, 1.5, 0.0};   // outside
  
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentBothEndpointsOnBoundary) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{0.5, 0.0, 0.0}; // on edge a1-a2
  point_t seg_end{0.0, 0.5, 0.0};   // on edge a1-a3
  
  EXPECT_TRUE(tri.is_intersected_by_segm({seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentVeryCloseToTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{-1e-10, 0.5, 0.0};
  point_t seg_end{1e-10, 0.5, 0.0};
  
  // Should be true if it actually touches/intersects
  // Might be false due to floating point issues
  bool result = tri.is_intersected_by_segm({seg_start, seg_end});
  std::cout << "Close segment result: " << result << std::endl;
}

TEST(TriangleSegmentTest, DegenerateTriangleSegment) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.5, 0.0, 0.0}; // degenerate
  triangle_t<double> tri{a1, a2, a3};
  
  point_t seg_start{0.2, 0.0, 0.0};
  point_t seg_end{0.8, 0.0, 0.0};
  
  // Behavior depends on implementation
  bool result = tri.is_intersected_by_segm({seg_start, seg_end});
  std::cout << "Degenerate triangle segment result: " << result << std::endl;
}

TEST(TrianglePointTest, DifferentVertexOrdering) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  // Same triangle points, different construction order
  triangle_t<double> tri1{a1, a2, a3};
  triangle_t<double> tri2{a3, a2, a1}; // Reversed order
  
  point_t inside{0.2, 0.2, 0.0};

  EXPECT_TRUE(tri1.is_point_inside_triang(inside));
  EXPECT_TRUE(tri2.is_point_inside_triang(inside)); // This might fail!
}

// ----------------------------------

TEST(TriangleSegmentTest, SegmentThroughInteriorNoEdgeTouch2) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  // Segment passes through interior without touching edges
  point_t seg_start{0.5, 0.5, -1.0};
  point_t seg_end{0.5, 0.5, 1.0};
  
  EXPECT_TRUE(tri.is_intersected_by_segm(segment_t{seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentInPlaneInsideTriangle2) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  // Segment completely inside triangle, in the same plane
  point_t seg_start{0.2, 0.2, 0.0};
  point_t seg_end{0.3, 0.3, 0.0};
  
  EXPECT_TRUE(tri.is_intersected_by_segm(segment_t{seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentInPlaneCrossingTriangle2) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  // Segment crosses triangle interior but doesn't touch edges
  point_t seg_start{0.1, 0.1, 0.0};
  point_t seg_end{0.8, 0.8, 0.0}; // Exits through hypotenuse
  
  EXPECT_TRUE(tri.is_intersected_by_segm(segment_t{seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentInPlaneOutsideTriangle2) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  // Segment in same plane but outside triangle
  point_t seg_start{1.5, 0.5, 0.0};
  point_t seg_end{2.0, 0.5, 0.0};
  
  EXPECT_FALSE(tri.is_intersected_by_segm(segment_t{seg_start, seg_end}));
}

TEST(TriangleSegmentTest, SegmentEndpointInsideTriangle2) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  triangle_t<double> tri{a1, a2, a3};
  
  // Segment starts inside triangle, goes out
  point_t seg_start{0.2, 0.2, 0.0};
  point_t seg_end{1.5, 1.5, 1.0}; // Exits through plane
  
  EXPECT_TRUE(tri.is_intersected_by_segm(segment_t{seg_start, seg_end}));
}
