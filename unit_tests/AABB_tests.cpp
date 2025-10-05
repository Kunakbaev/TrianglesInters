#include <gtest/gtest.h>
#include "BVH_solution/AABB.hpp"

TEST(AABBTest, IntersectionIdenticalBoxes) {
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb1(triangle);
  AABB_t<double> aabb2(triangle);
  
  EXPECT_TRUE(aabb1.does_inter(aabb2));
  EXPECT_TRUE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, IntersectionOverlappingBoxes) {
  // First triangle: (0,0,0), (1,1,1), (0,1,0)
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  triangle_t<double> triangle1(p1, p2, p3);
  AABB_t<double> aabb1(triangle1);
  
  // Second triangle: (0.5,0.5,0.5), (1.5,1.5,1.5), (0.5,1.5,0.5)
  point_t<double> p4(0.5, 0.5, 0.5);
  point_t<double> p5(1.5, 1.5, 1.5);
  point_t<double> p6(0.5, 1.5, 0.5);
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  EXPECT_TRUE(aabb1.does_inter(aabb2));
  EXPECT_TRUE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, NoIntersectionSeparatedBoxes) {
  // First triangle: (0,0,0), (1,1,1), (0,1,0)
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  triangle_t<double> triangle1(p1, p2, p3);
  AABB_t<double> aabb1(triangle1);
  
  // Second triangle completely separated in x-axis
  point_t<double> p4(2.0, 0.0, 0.0);
  point_t<double> p5(3.0, 1.0, 1.0);
  point_t<double> p6(2.0, 1.0, 0.0);
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  EXPECT_FALSE(aabb1.does_inter(aabb2));
  EXPECT_FALSE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, NoIntersectionSeparatedInY) {
  // First triangle: (0,0,0), (1,1,1), (0,1,0)
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  triangle_t<double> triangle1(p1, p2, p3);
  AABB_t<double> aabb1(triangle1);
  
  // Second triangle separated in y-axis
  point_t<double> p4(0.0, 2.0, 0.0);
  point_t<double> p5(1.0, 3.0, 1.0);
  point_t<double> p6(0.0, 3.0, 0.0);
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  EXPECT_FALSE(aabb1.does_inter(aabb2));
  EXPECT_FALSE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, NoIntersectionSeparatedInZ) {
  // First triangle: (0,0,0), (1,1,1), (0,1,0)
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  triangle_t<double> triangle1(p1, p2, p3);
  AABB_t<double> aabb1(triangle1);
  
  // Second triangle separated in z-axis
  point_t<double> p4(0.0, 0.0, 2.0);
  point_t<double> p5(1.0, 1.0, 3.0);
  point_t<double> p6(0.0, 1.0, 2.0);
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  EXPECT_FALSE(aabb1.does_inter(aabb2));
  EXPECT_FALSE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, TouchingBoxes) {
  // First triangle: (0,0,0), (1,1,1), (0,1,0)
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  triangle_t<double> triangle1(p1, p2, p3);
  AABB_t<double> aabb1(triangle1);
  
  // Second triangle touching exactly at x=1
  point_t<double> p4(1.0, 0.0, 0.0);
  point_t<double> p5(2.0, 1.0, 1.0);
  point_t<double> p6(1.0, 1.0, 0.0);
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  // This depends on whether your intersection test considers touching as intersection
  // Most AABB tests consider touching as intersection since boundaries are inclusive
  EXPECT_TRUE(aabb1.does_inter(aabb2));
  EXPECT_TRUE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, SinglePointTriangle) {
  point_t<double> p1(1.0, 1.0, 1.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(1.0, 1.0, 1.0);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  // AABB of single point should be valid
  // Test intersection with another single point
  point_t<double> p4(1.0, 1.0, 1.0);
  point_t<double> p5(1.0, 1.0, 1.0);
  point_t<double> p6(1.0, 1.0, 1.0);
  
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  EXPECT_TRUE(aabb.does_inter(aabb2));
}

TEST(AABBTest, DegenerateTriangle) {
  // Triangle with collinear points
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(2.0, 2.0, 2.0);
  point_t<double> p3(1.0, 1.0, 1.0);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  // Should still create a valid AABB from min(0,0,0) to max(2,2,2)
}




// --------   CHECK INNER STATE BY TESTING GET_MIN/MAX_CORNER METHODS   -------

TEST(AABBTest, ConstructorFromTriangleGetCorner) {
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 2.0, 3.0);
  point_t<double> p3(-1.0, 1.0, 0.5);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  // Test the getter methods
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  EXPECT_DOUBLE_EQ(min_corner.get_x(), -1.0);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 0.0);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 0.0);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 1.0);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 2.0);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 3.0);
}

TEST(AABBTest, GetMinMaxCornersSimpleTriangleGetCorner) {
  point_t<double> p1(1.0, 1.0, 1.0);
  point_t<double> p2(3.0, 3.0, 3.0);
  point_t<double> p3(2.0, 0.5, 1.5);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  EXPECT_DOUBLE_EQ(min_corner.get_x(), 1.0);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 0.5);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 1.0);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 3.0);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 3.0);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 3.0);
}

TEST(AABBTest, GetMinMaxCornersAllPositiveGetCorner) {
  point_t<double> p1(1.0, 2.0, 3.0);
  point_t<double> p2(4.0, 5.0, 6.0);
  point_t<double> p3(2.0, 3.0, 7.0);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  EXPECT_DOUBLE_EQ(min_corner.get_x(), 1.0);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 2.0);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 3.0);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 4.0);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 5.0);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 7.0);
}

TEST(AABBTest, GetMinMaxCornersAllNegative) {
  point_t<double> p1(-1.0, -2.0, -3.0);
  point_t<double> p2(-4.0, -5.0, -6.0);
  point_t<double> p3(-2.0, -3.0, -1.0);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  EXPECT_DOUBLE_EQ(min_corner.get_x(), -4.0);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), -5.0);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), -6.0);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), -1.0);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), -2.0);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), -1.0);
}

TEST(AABBTest, GetMinMaxCornersMixedSigns) {
  point_t<double> p1(-1.0, 2.0, -3.0);
  point_t<double> p2(3.0, -4.0, 5.0);
  point_t<double> p3(0.0, 1.0, -1.0);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  EXPECT_DOUBLE_EQ(min_corner.get_x(), -1.0);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), -4.0);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), -3.0);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 3.0);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 2.0);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 5.0);
}

TEST(AABBTest, IntersectionIdenticalBoxesGetCorner) {
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb1(triangle);
  AABB_t<double> aabb2(triangle);
  
  // Verify corners are identical
  EXPECT_TRUE(aabb1.get_min_corner() == aabb2.get_min_corner());
  EXPECT_TRUE(aabb1.get_max_corner() == aabb2.get_max_corner());
  
  EXPECT_TRUE(aabb1.does_inter(aabb2));
  EXPECT_TRUE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, IntersectionOverlappingBoxesGetCorner) {
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  triangle_t<double> triangle1(p1, p2, p3);
  AABB_t<double> aabb1(triangle1);
  
  point_t<double> p4(0.5, 0.5, 0.5);
  point_t<double> p5(1.5, 1.5, 1.5);
  point_t<double> p6(0.5, 1.5, 0.5);
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  // Verify the corners are as expected
  EXPECT_DOUBLE_EQ(aabb1.get_min_corner().get_x(), 0.0);
  EXPECT_DOUBLE_EQ(aabb2.get_min_corner().get_x(), 0.5);
  
  EXPECT_TRUE(aabb1.does_inter(aabb2));
  EXPECT_TRUE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, NoIntersectionSeparatedBoxesGetCorner) {
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(1.0, 1.0, 1.0);
  point_t<double> p3(0.0, 1.0, 0.0);
  triangle_t<double> triangle1(p1, p2, p3);
  AABB_t<double> aabb1(triangle1);
  
  point_t<double> p4(2.0, 0.0, 0.0);
  point_t<double> p5(3.0, 1.0, 1.0);
  point_t<double> p6(2.0, 1.0, 0.0);
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  // Verify boxes don't overlap by checking corners
  EXPECT_TRUE(aabb1.get_max_corner().get_x() < aabb2.get_min_corner().get_x());
  
  EXPECT_FALSE(aabb1.does_inter(aabb2));
  EXPECT_FALSE(aabb2.does_inter(aabb1));
}

TEST(AABBTest, SinglePointTriangleCorners) {
  point_t<double> p1(5.0, 5.0, 5.0);
  point_t<double> p2(5.0, 5.0, 5.0);
  point_t<double> p3(5.0, 5.0, 5.0);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  // For single point, min and max should be identical
  EXPECT_DOUBLE_EQ(min_corner.get_x(), 5.0);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 5.0);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 5.0);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 5.0);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 5.0);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 5.0);
  
  EXPECT_TRUE(min_corner == max_corner);
}

TEST(AABBTest, DegenerateTriangleCorners) {
  point_t<double> p1(0.0, 0.0, 0.0);
  point_t<double> p2(5.0, 5.0, 5.0);
  point_t<double> p3(2.5, 2.5, 2.5);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  EXPECT_DOUBLE_EQ(min_corner.get_x(), 0.0);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 0.0);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 0.0);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 5.0);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 5.0);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 5.0);
}


// --------- DEGENERATE TRIANGLES CASES + MORE PRECISE DOUBLE COORDS -------

TEST(AABBTest, PreciseCoordinatesThreeDecimalPlaces) {
  point_t<double> p1(1.234, 2.345, 3.456);
  point_t<double> p2(4.567, 5.678, 6.789);
  point_t<double> p3(0.123, 3.456, 2.345);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  EXPECT_DOUBLE_EQ(min_corner.get_x(), 0.123);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 2.345);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 2.345);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 4.567);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 5.678);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 6.789);
}

TEST(AABBTest, DegenerateTriangleSinglePoint) {
  // All three points are identical - degenerate to a point
  point_t<double> p1(2.718, 3.142, 1.618);
  point_t<double> p2(2.718, 3.142, 1.618);
  point_t<double> p3(2.718, 3.142, 1.618);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  // For single point, min and max should be identical
  EXPECT_DOUBLE_EQ(min_corner.get_x(), 2.718);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 3.142);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 1.618);
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 2.718);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 3.142);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 1.618);
}

TEST(AABBTest, DegenerateTriangleCollinearPoints) {
  // Three collinear points - degenerate to a line segment
  point_t<double> p1(0.111, 0.222, 0.333);
  point_t<double> p2(0.555, 1.110, 1.665);
  point_t<double> p3(0.333, 0.666, 0.999);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  // Should create AABB from min to max of all points
  EXPECT_DOUBLE_EQ(min_corner.get_x(), 0.111);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 0.222);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 0.333);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 0.555);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 1.110);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 1.665);
}

TEST(AABBTest, VerySmallTrianglePreciseCoordinates) {
  // Very small triangle with precise coordinates
  point_t<double> p1(100.001, 200.002, 300.003);
  point_t<double> p2(100.002, 200.001, 300.004);
  point_t<double> p3(100.001, 200.003, 300.002);
  
  triangle_t<double> triangle(p1, p2, p3);
  AABB_t<double> aabb(triangle);
  
  point_t<double> min_corner = aabb.get_min_corner();
  point_t<double> max_corner = aabb.get_max_corner();
  
  EXPECT_DOUBLE_EQ(min_corner.get_x(), 100.001);
  EXPECT_DOUBLE_EQ(min_corner.get_y(), 200.001);
  EXPECT_DOUBLE_EQ(min_corner.get_z(), 300.002);
  
  EXPECT_DOUBLE_EQ(max_corner.get_x(), 100.002);
  EXPECT_DOUBLE_EQ(max_corner.get_y(), 200.003);
  EXPECT_DOUBLE_EQ(max_corner.get_z(), 300.004);
}

TEST(AABBTest, MixedPreciseCoordinatesIntersectionTest) {
  // Test intersection with precise coordinates
  point_t<double> p1(1.111, 2.222, 3.333);
  point_t<double> p2(3.333, 4.444, 5.555);
  point_t<double> p3(1.111, 4.444, 3.333);
  triangle_t<double> triangle1(p1, p2, p3);
  AABB_t<double> aabb1(triangle1);
  
  // Second triangle that slightly overlaps
  point_t<double> p4(3.000, 4.000, 5.000);
  point_t<double> p5(4.000, 5.000, 6.000);
  point_t<double> p6(3.000, 5.000, 5.000);
  triangle_t<double> triangle2(p4, p5, p6);
  AABB_t<double> aabb2(triangle2);
  
  // Verify corners
  EXPECT_DOUBLE_EQ(aabb1.get_max_corner().get_x(), 3.333);
  EXPECT_DOUBLE_EQ(aabb2.get_min_corner().get_x(), 3.000);
  
  // They should intersect since aabb1 max(3.333) > aabb2 min(3.000)
  EXPECT_TRUE(aabb1.does_inter(aabb2));
  EXPECT_TRUE(aabb2.does_inter(aabb1));
}
