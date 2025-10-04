#include <gtest/gtest.h>
#include "point.hpp"
#include "triangle.hpp"
#include "bruteforce_solution.hpp"

TEST(BruteforceSolutionTest, EmptyInput) {
  std::vector<triangle_t<double>> triangles;
  bruteforce_solution_t solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_TRUE(result.empty());
}

TEST(BruteforceSolutionTest, SingleTriangle) {
  point_t p1{0.0, 0.0, 0.0};
  point_t p2{1.0, 0.0, 0.0};
  point_t p3{0.0, 1.0, 0.0};
  
  std::vector<triangle_t<double>> triangles{triangle_t<double>{p1, p2, p3}};
  bruteforce_solution_t solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_TRUE(result.empty());
}

TEST(BruteforceSolutionTest, TwoIntersectingTriangles) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 1.0, 0.0};
  point_t b2{3.0, 1.0, 0.0};
  point_t b3{1.0, 3.0, 0.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3}
  };
  bruteforce_solution_t solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  EXPECT_EQ(result.size(), 2);
  EXPECT_NE(std::find(result.begin(), result.end(), 0), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 1), result.end());
}

TEST(BruteforceSolutionTest, MixedIntersectingAndNonIntersecting) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.5, 0.5, 0.0};
  point_t b2{1.5, 0.5, 0.0};
  point_t b3{0.5, 1.5, 0.0};
  
  point_t c1{3.0, 3.0, 3.0};
  point_t c2{4.0, 3.0, 3.0};
  point_t c3{3.0, 4.0, 3.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3},
    triangle_t<double>{c1, c2, c3}
  };
  bruteforce_solution_t solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  EXPECT_EQ(result.size(), 2);
  EXPECT_NE(std::find(result.begin(), result.end(), 0), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 1), result.end());
  EXPECT_EQ(std::find(result.begin(), result.end(), 2), result.end());
}

TEST(BruteforceSolutionTest, AllTrianglesIntersecting) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 1.0, 0.0};
  point_t b2{3.0, 1.0, 0.0};
  point_t b3{1.0, 3.0, 0.0};
  
  point_t c1{0.5, 0.5, 0.0};
  point_t c2{1.5, 0.5, 0.0};
  point_t c3{0.5, 1.5, 0.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3},
    triangle_t<double>{c1, c2, c3}
  };
  bruteforce_solution_t solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  EXPECT_EQ(result.size(), 3);
  EXPECT_NE(std::find(result.begin(), result.end(), 0), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 1), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 2), result.end());
}

TEST(BruteforceSolutionTest, NoTrianglesIntersecting) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{3.0, 0.0, 0.0};
  point_t b2{4.0, 0.0, 0.0};
  point_t b3{3.0, 1.0, 0.0};
  
  point_t c1{0.0, 3.0, 0.0};
  point_t c2{1.0, 3.0, 0.0};
  point_t c3{0.0, 4.0, 0.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3},
    triangle_t<double>{c1, c2, c3}
  };
  bruteforce_solution_t solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  EXPECT_TRUE(result.empty());
}