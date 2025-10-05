#include <cmath>
#include <gtest/gtest.h>

#include "geom_primitives/utils.hpp"

TEST(Sign, Negative) {
  EXPECT_EQ(utils::sign(-1.0), -1);
}

TEST(Sign, NegativeCloseTo0) {
  EXPECT_EQ(utils::sign(-1e-5), -1);
}

TEST(Sign, Zero) {
  EXPECT_EQ(utils::sign(0.0), 0);
}

TEST(Sign, CloseTo0) {
  EXPECT_EQ(utils::sign(-1e-9), 0);
}

TEST(Sign, Positive) {
  EXPECT_EQ(utils::sign(1.0), 1);
}

TEST(Sign, PositiveCloseTo0) {
  EXPECT_EQ(utils::sign(1e-5), 1);
}

//  ----------------------------------

TEST(Square, Normal) {
  EXPECT_DOUBLE_EQ(utils::sqr(10.0), std::pow(10.0, 2));
}

TEST(Square, Small) {
  EXPECT_DOUBLE_EQ(utils::sqr(1e-3), std::pow(1e-3, 2));
}

TEST(Square, Large) {
  EXPECT_DOUBLE_EQ(utils::sqr(1e8), std::pow(1e8, 2));
}
