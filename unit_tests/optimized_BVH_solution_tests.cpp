#include <gtest/gtest.h>
#include <random>

#include "point.hpp"
#include "triangle.hpp"
#include "BVH_solution/solution.hpp"

TEST(BVHFastSolutionTest, EmptyInput) {
  std::vector<triangle_t<double>> triangles;
  BVH_fast_solution_t<double> solver{triangles};
  
  std::cerr << std::endl;
  auto result = solver.get_inter_triangs_indices();
  EXPECT_TRUE(result.empty());
}

TEST(BVHFastSolutionTest, SingleTriangle) {
  point_t p1{0.0, 0.0, 0.0};
  point_t p2{1.0, 0.0, 0.0};
  point_t p3{0.0, 1.0, 0.0};
  
  std::vector<triangle_t<double>> triangles{triangle_t<double>{p1, p2, p3}};
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_TRUE(result.empty());
}

TEST(BVHFastSolutionTest, TwoIntersectingTriangles) {
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
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  EXPECT_EQ(result.size(), 2);
  EXPECT_NE(std::find(result.begin(), result.end(), 0), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 1), result.end());
}

TEST(BVHFastSolutionTest, MixedIntersectingAndNonIntersecting) {
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
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  EXPECT_EQ(result.size(), 2);
  EXPECT_NE(std::find(result.begin(), result.end(), 0), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 1), result.end());
  EXPECT_EQ(std::find(result.begin(), result.end(), 2), result.end());
}

TEST(BVHFastSolutionTest, AllTrianglesIntersecting) {
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
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  EXPECT_EQ(result.size(), 3);
  EXPECT_NE(std::find(result.begin(), result.end(), 0), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 1), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 2), result.end());
}

TEST(BVHFastSolutionTest, NoTrianglesIntersecting) {
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
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  EXPECT_TRUE(result.empty());
}

TEST(BVHFastSolutionTest, CoplanarIntersectingTriangles) {
  point_t a1{-1.0, 1.0, 0.0};
  point_t a2{1.0, 1.0, 0.0};
  point_t a3{0.0, -1.0, 0.0};
  
  point_t b1{0.0, 1.0, 0.0};
  point_t b2{-1.0, -1.0, 0.0};
  point_t b3{1.0, -1.0, 0.0};
  
  point_t c1{100.0, 100.0, 100.0};
  point_t c2{100.0, 100.0, 100.0};
  point_t c3{100.0, 100.0, 100.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3},
    triangle_t<double>{c1, c2, c3}
  };
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  
  std::cerr << "HELLO" << std::endl;

  EXPECT_EQ(result.size(), 2);
  EXPECT_NE(std::find(result.begin(), result.end(), 0), result.end());
  EXPECT_NE(std::find(result.begin(), result.end(), 1), result.end());
  EXPECT_EQ(std::find(result.begin(), result.end(), 2), result.end());
}

TEST(BVHFastSolutionTest, DegenerateTriangle) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.5, 0.5, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{0.0, 0.0, 0.0};
  point_t b3{0.0, 0.0, 0.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3}
  };
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_EQ(result.size(), 2);
}

TEST(BVHFastSolutionTest, SharedEdgeButNotIntersecting) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.0, 0.0, 0.0};
  point_t b2{1.0, 0.0, 0.0};
  point_t b3{0.0, -1.0, 0.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3}
  };
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_EQ(result.size(), 2);
}

TEST(BVHFastSolutionTest, TriangleInsideAnother) {
  point_t outer1{0.0, 0.0, 0.0};
  point_t outer2{3.0, 0.0, 0.0};
  point_t outer3{0.0, 3.0, 0.0};
  
  point_t inner1{1.0, 1.0, 0.0};
  point_t inner2{2.0, 1.0, 0.0};
  point_t inner3{1.0, 2.0, 0.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{outer1, outer2, outer3},
    triangle_t<double>{inner1, inner2, inner3}
  };
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_EQ(result.size(), 2);
}

TEST(BVHFastSolutionTest, ZFightingPrecision) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{1.0, 0.0, 0.0};
  point_t a3{0.0, 1.0, 0.0};
  
  point_t b1{0.0, 0.0, 1e-10};
  point_t b2{1.0, 0.0, 1e-10};
  point_t b3{0.0, 1.0, 1e-10};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3}
  };
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_EQ(result.size(), 2);
}

TEST(BVHFastSolutionTest, LargeCoordinates) {
  point_t a1{1e6, 1e6, 1e6};
  point_t a2{1e6 + 1.0, 1e6, 1e6};
  point_t a3{1e6, 1e6 + 1.0, 1e6};
  
  point_t b1{1e6 + 0.5, 1e6 + 0.5, 1e6};
  point_t b2{1e6 + 1.5, 1e6 + 0.5, 1e6};
  point_t b3{1e6 + 0.5, 1e6 + 1.5, 1e6};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3}
  };
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_EQ(result.size(), 2);
}

TEST(BVHFastSolutionTest, IntersectingAtSinglePoint) {
  point_t a1{0.0, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.0, 2.0, 0.0};
  
  point_t b1{1.0, 1.0, 0.0};
  point_t b2{1.0, 3.0, 0.0};
  point_t b3{3.0, 1.0, 0.0};
  
  std::vector<triangle_t<double>> triangles{
    triangle_t<double>{a1, a2, a3},
    triangle_t<double>{b1, b2, b3}
  };
  BVH_fast_solution_t<double> solver{triangles};
  
  auto result = solver.get_inter_triangs_indices();
  EXPECT_EQ(result.size(), 2);
}

// -----------------------------------------

TEST(BVHFastSolutionTest, DenseGridWithPartialOverlaps) {
  std::vector<triangle_t<double>> triangles;
  
  // Create a dense grid of triangles with various orientations
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      // First triangle in cell - oriented one way
      point_t p1{i * 0.8, j * 0.8, 0.0};
      point_t p2{i * 0.8 + 1.0, j * 0.8, 0.0};
      point_t p3{i * 0.8, j * 0.8 + 1.0, 0.0};
      triangles.emplace_back(p1, p2, p3);
      
      // Second triangle in cell - oriented differently, overlapping
      point_t p4{i * 0.8 + 0.2, j * 0.8 + 0.2, 0.1};
      point_t p5{i * 0.8 + 1.2, j * 0.8 + 0.2, 0.1};
      point_t p6{i * 0.8 + 0.2, j * 0.8 + 1.2, 0.1};
      triangles.emplace_back(p4, p5, p6);
      
      // Third triangle - crossing cell boundaries
      point_t p7{i * 0.8 + 0.5, j * 0.8 + 0.5, -0.1};
      point_t p8{i * 0.8 + 1.5, j * 0.8 + 0.5, -0.1};
      point_t p9{i * 0.8 + 0.5, j * 0.8 + 1.5, -0.1};
      triangles.emplace_back(p7, p8, p9);
    }
  }
  
  BVH_fast_solution_t<double> solver{triangles};
  auto result = solver.get_inter_triangs_indices();
  
  // Should have many intersections due to dense packing
  EXPECT_GT(result.size(), triangles.size() / 2);
}

TEST(BVHFastSolutionTest, IntersectingThinSlivers) {
  std::vector<triangle_t<double>> triangles;
  
  // Create many thin, long triangles that intersect at sharp angles
  for (int i = 0; i < 10; ++i) {
    double angle = i * M_PI / 10;
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    
    // Very thin triangle radiating from center
    point_t p1{0.0, 0.0, 0.0};
    point_t p2{cos_a * 2.0, sin_a * 2.0, 0.0};
    point_t p3{cos_a * 2.0 + sin_a * 0.01, sin_a * 2.0 - cos_a * 0.01, 0.0};
    
    triangles.emplace_back(p1, p2, p3);
  }
  
  // Add crossing thin triangles
  for (int i = 0; i < 10; ++i) {
    double offset = i * 0.2 - 1.0;
    point_t p1{offset, -1.0, 0.0};
    point_t p2{offset, 1.0, 0.0};
    point_t p3{offset + 0.01, 1.0, 0.0};
    
    triangles.emplace_back(p1, p2, p3);
  }
  
  BVH_fast_solution_t<double> solver{triangles};
  auto result = solver.get_inter_triangs_indices();
  
  // All radial triangles intersect with many vertical ones
  EXPECT_GT(result.size(), 15);
}

TEST(BVHFastSolutionTest, NestedHierarchyOfIntersections) {
  std::vector<triangle_t<double>> triangles;
  
  // Large outer triangle
  point_t outer1{-5.0, -5.0, 0.0};
  point_t outer2{5.0, -5.0, 0.0};
  point_t outer3{-5.0, 5.0, 0.0};
  triangles.emplace_back(outer1, outer2, outer3);
  
  // Multiple levels of nested triangles
  for (int level = 1; level <= 4; ++level) {
    double size = 10.0 / level;
    for (int i = 0; i < level; ++i) {
      for (int j = 0; j < level; ++j) {
        double x = -5.0 + i * (10.0 / level);
        double y = -5.0 + j * (10.0 / level);
        
        point_t p1{x, y, level * 0.1};
        point_t p2{x + size, y, level * 0.1};
        point_t p3{x, y + size, level * 0.1};
        triangles.emplace_back(p1, p2, p3);
        
        // Add crossing triangle
        point_t p4{x + size/2, y + size/2, level * 0.1 + 0.05};
        point_t p5{x + size/2 + size, y + size/2, level * 0.1 + 0.05};
        point_t p6{x + size/2, y + size/2 + size, level * 0.1 + 0.05};
        triangles.emplace_back(p4, p5, p6);
      }
    }
  }
  
  BVH_fast_solution_t<double> solver{triangles};
  auto result = solver.get_inter_triangs_indices();
  
  // Complex nested structure should have many intersections
  EXPECT_GT(result.size(), 20);
}

TEST(BVHFastSolutionTest, PrecisionEdgeCases) {
  std::vector<triangle_t<double>> triangles;
  
  // Triangles that are extremely close but not quite touching
  for (int i = 0; i < 5; ++i) {
    double eps = 1e-10 * i;
    
    point_t p1{0.0 + eps, 0.0 + eps, 0.0 + eps};
    point_t p2{1.0 + eps, 0.0 + eps, 0.0 + eps};
    point_t p3{0.0 + eps, 1.0 + eps, 0.0 + eps};
    triangles.emplace_back(p1, p2, p3);
  }
  
  // Triangles that should barely intersect
  point_t a1{0.9999999999, 0.0, 0.0};
  point_t a2{2.0, 0.0, 0.0};
  point_t a3{0.9999999999, 2.0, 0.0};
  triangles.emplace_back(a1, a2, a3);
  
  point_t b1{1.0, 0.0, 0.0};
  point_t b2{1.0, 2.0, 0.0};
  point_t b3{3.0, 0.0, 0.0};
  triangles.emplace_back(b1, b2, b3);
  
  BVH_fast_solution_t<double> solver{triangles};
  auto result = solver.get_inter_triangs_indices();
  
  // The precision triangles should intersect with each other
  // and the edge case should be detected
  EXPECT_GT(result.size(), 3);
}

TEST(BVHFastSolutionTest, MixedDegenerateAndValid) {
  std::vector<triangle_t<double>> triangles;
  
  // Valid triangles
  for (int i = 0; i < 3; ++i) {
    point_t p1{i * 1.0, 0.0, 0.0};
    point_t p2{i * 1.0 + 1.0, 0.0, 0.0};
    point_t p3{i * 1.0, 1.0, 0.0};
    triangles.emplace_back(p1, p2, p3);
  }
  
  // Nearly degenerate triangles (very small area)
  for (int i = 0; i < 3; ++i) {
    point_t p1{0.5 + i * 0.1, 0.5, 0.0};
    point_t p2{0.5 + i * 0.1 + 1e-12, 0.5, 0.0};
    point_t p3{0.5 + i * 0.1, 0.5 + 1e-12, 0.0};
    triangles.emplace_back(p1, p2, p3);
  }
  
  // Actually degenerate triangles (colinear points)
  point_t d1{2.0, 2.0, 0.0};
  point_t d2{2.0, 2.0, 0.0};
  point_t d3{3.0, 3.0, 0.0};
  triangles.emplace_back(d1, d2, d3);
  
  point_t d4{2.1, 2.0, 0.0};
  point_t d5{2.2, 2.0, 0.0};
  point_t d6{2.3, 2.0, 0.0};
  triangles.emplace_back(d4, d5, d6);
  
  BVH_fast_solution_t<double> solver{triangles};
  auto result = solver.get_inter_triangs_indices();
  
  // Should handle degenerate cases without crashing
  // and still detect valid intersections
  EXPECT_GE(result.size(), 2);
}

TEST(BVHFastSolutionTest, Complex3DWeaving) {
  std::vector<triangle_t<double>> triangles;
  
  // Create triangles that weave through each other in 3D space
  for (int i = 0; i < 8; ++i) {
    double t = i * M_PI / 4;

    // Horizontal triangle at varying heights
    point_t p1{cos(t) * 2.0, sin(t) * 2.0, i * 0.2};
    point_t p2{cos(t + M_PI/2) * 2.0, sin(t + M_PI/2) * 2.0, i * 0.2};
    point_t p3{cos(t + M_PI) * 2.0, sin(t + M_PI) * 2.0, i * 0.2};
    triangles.emplace_back(p1, p2, p3);

    // Vertical triangle crossing through
    point_t p4{cos(t) * 1.0, sin(t) * 1.0, -1.0};
    point_t p5{cos(t) * 1.0, sin(t) * 1.0, 2.0};
    point_t p6{cos(t + M_PI/4) * 1.0, sin(t + M_PI/4) * 1.0, 0.5};
    triangles.emplace_back(p4, p5, p6);
  }
  
  BVH_fast_solution_t<double> solver{triangles};
  auto result = solver.get_inter_triangs_indices();

  // Complex 3D weaving should produce many intersections
  EXPECT_GT(result.size(), 10);
}


