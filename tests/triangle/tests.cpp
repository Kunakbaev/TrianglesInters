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
