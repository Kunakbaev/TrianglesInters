#include <gtest/gtest.h>

#include "geom_primitives/point.hpp"

//  ----------------------------------

TEST(IsZero, ZeroVector) {
  vector_t<double> vec(0, 0, 0);
  EXPECT_TRUE(vec.is_zero());
}

TEST(IsZero, NotZeroVector) {
  vector_t<double> vec(0, 1, 0);
  EXPECT_FALSE(vec.is_zero());
}

TEST(IsZero, Close2ZeroVector) {
  vector_t<double> vec(1e-5, 0, 0); // carefull with EPS
  EXPECT_FALSE(vec.is_zero());
}

TEST(IsZero, VeryClose2ZeroVector) {
  vector_t<double> vec(1e-8, 0, 0); // carefull with EPS
  EXPECT_TRUE(vec.is_zero());
}

//  ----------------------------------

TEST(GetLen, PerfectSquare) {
  vector_t<double> vec(3, 4, 0);
  EXPECT_DOUBLE_EQ(vec.get_len_sq(), 25.0);
  EXPECT_DOUBLE_EQ(vec.get_len(),    5.0);
}

TEST(GetLen, Zero) {
  vector_t<double> vec(0, 0, 0);
  EXPECT_DOUBLE_EQ(vec.get_len_sq(), 0.0);
  EXPECT_DOUBLE_EQ(vec.get_len(),    0.0);
}

TEST(GetLen, Sqrt3) {
  vector_t<double> vec(1, 1, 1);
  EXPECT_DOUBLE_EQ(vec.get_len_sq(), 3.0);
  EXPECT_DOUBLE_EQ(vec.get_len(),    sqrtl(3.0));
}

TEST(GetLen, Sqrt7665) {
  vector_t<double> vec(10, 82, 29);
  EXPECT_DOUBLE_EQ(vec.get_len_sq(), 7665.0);
  EXPECT_DOUBLE_EQ(vec.get_len(),    sqrtl(7665.0));
}

//  ----------------------------------

TEST(OperatorEq, Equal) {
  vector_t<double> one(1.281, 1.0912, 1.761);
  vector_t<double> two(1.281, 1.0912, 1.761);
  EXPECT_EQ(one, two);
}

TEST(OperatorEq, NotEqual) {
  vector_t<double> one(1.1, 1.21, 1);
  vector_t<double> two(1.39, 0, 1.1);
  EXPECT_FALSE(one == two);
}

//  ----------------------------------

TEST(OperatorPlus, ZeroVec) {
  vector_t<double> vec(0, 0, 0);
  vector_t<double> vec2(1.21, 5.1, -1.189);
  EXPECT_EQ(vec + vec2, vec2);
}

TEST(OperatorPlus, NonZeroVec) {
  vector_t<double> vec(-1.1, 1, 3.19);
  vector_t<double> vec2(1, 5, 1);
  vector_t<double> expected(-0.1, 6, 4.19);
  EXPECT_EQ(vec + vec2, expected);
}

//  ----------------------------------

TEST(DotProduct, Ort) {
  vector_t<double> vec(-1, 1, 2);
  vector_t<double> vec2(1, 3, -1);
  EXPECT_DOUBLE_EQ(vec_ops::dot(vec, vec2), 0.0);
}

TEST(DotProduct, NotOrt) {
  vector_t<double> vec(-1, 1.19, 2);
  vector_t<double> vec2(1.2, 2, -1.89);
  EXPECT_DOUBLE_EQ(vec_ops::dot(vec, vec2), -2.6);
}

//  ----------------------------------

TEST(CrossProduct, One) {
  vector_t<double> vec(1, 0, 0);
  vector_t<double> vec2(0, 1, 0);
  vector_t<double> expected(0, 0, 1);
  EXPECT_EQ(vec_ops::cross(vec, vec2), expected);
}

TEST(CrossProduct, Two) {
  vector_t<double> vec (1.32, 1.98, 12.12);
  vector_t<double> vec2(-8, -0.39, 7);
  vector_t<double> expected(18.5868, -106.2, 15.3252);
  EXPECT_EQ(vec_ops::cross(vec, vec2), expected);
}

//  ----------------------------------

TEST(Stream, Input) {
  std::stringstream stream("1.29 -292.121 9.1");
  point_t<double> point;
  stream >> point;
  point_t<double> expected(1.29, -292.121, 9.1);
  EXPECT_EQ(point, expected);
}

// ----------- test methods get_max/min_of_2_points and get_coord_by_axis_name ---

TEST(PointOperationsTest, GetMaxOf2PointsBasic) {
  point_t<double> p1{1.0, 2.0, 3.0};
  point_t<double> p2{4.0, 5.0, 6.0};
  
  auto result = vec_ops::get_max_of_2_points(p1, p2);
  
  EXPECT_DOUBLE_EQ(4.0, result.x);
  EXPECT_DOUBLE_EQ(5.0, result.y);
  EXPECT_DOUBLE_EQ(6.0, result.z);
}

TEST(PointOperationsTest, GetMinOf2PointsBasic) {
  point_t<double> p1{1.123, 2.456, 3.789};
  point_t<double> p2{4.321, 5.654, 6.987};
  
  auto result = vec_ops::get_min_of_2_points(p1, p2);
  
  EXPECT_DOUBLE_EQ(1.123, result.x);
  EXPECT_DOUBLE_EQ(2.456, result.y);
  EXPECT_DOUBLE_EQ(3.789, result.z);
}

TEST(PointOperationsTest, GetMaxOf2PointsMixedValues) {
  point_t<double> p1{2.500, 1.100, 5.750};
  point_t<double> p2{1.250, 3.300, 4.250};
  
  auto result = vec_ops::get_max_of_2_points(p1, p2);
  
  EXPECT_DOUBLE_EQ(2.500, result.x);  // p1.x is larger
  EXPECT_DOUBLE_EQ(3.300, result.y);  // p2.y is larger
  EXPECT_DOUBLE_EQ(5.750, result.z);  // p1.z is larger
}

TEST(PointOperationsTest, GetMinOf2PointsMixedValues) {
  point_t<double> p1{2.500, 1.100, 5.750};
  point_t<double> p2{1.250, 3.300, 4.250};
  
  auto result = vec_ops::get_min_of_2_points(p1, p2);
  
  EXPECT_DOUBLE_EQ(1.250, result.x);  // p2.x is smaller
  EXPECT_DOUBLE_EQ(1.100, result.y);  // p1.y is smaller
  EXPECT_DOUBLE_EQ(4.250, result.z);  // p2.z is smaller
}

TEST(PointOperationsTest, GetCoordByAxisName) {
  point_t<double> p{1.234, 5.678, 9.012};
  
  EXPECT_DOUBLE_EQ(1.234, p.get_coord_by_axis_name(utils::axis_t::X));
  EXPECT_DOUBLE_EQ(5.678, p.get_coord_by_axis_name(utils::axis_t::Y));
  EXPECT_DOUBLE_EQ(9.012, p.get_coord_by_axis_name(utils::axis_t::Z));
}
