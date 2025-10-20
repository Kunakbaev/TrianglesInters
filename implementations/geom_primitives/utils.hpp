#pragma once

#include <iostream>

namespace utils {
  inline const float       kEPS_f  = 1e-4;
  inline const double      kEPS_d  = 1e-6;
  inline const long double kEPS_ld = 1e-10;

  enum class signs_t {
    NEG  = -1,
    ZERO = 0,
    POS  = 1
  };

  template <typename U>
  [[nodiscard]] bool operator==(signs_t sign, U number) {
    return static_cast<U>(sign) == number;
  }

  enum class axis_t { X, Y, Z };

  [[nodiscard]] inline signs_t sign(float number) {
    if (number < -kEPS_f) {
      return signs_t::NEG;
    }

    return number > kEPS_f ? signs_t::POS : signs_t::ZERO;
  }

  [[nodiscard]] inline signs_t sign(double number) {
    if (number < -kEPS_d) {
      return signs_t::NEG;
    }

    return number > kEPS_d ? signs_t::POS : signs_t::ZERO;
  }

  [[nodiscard]] inline signs_t sign(long double number) {
    if (number < -kEPS_ld) {
      return signs_t::NEG;
    }

    return number > kEPS_ld ? signs_t::POS : signs_t::ZERO;
  }

  template<typename T>
  [[nodiscard]] T sqr(T x) { return x * x; }
};
