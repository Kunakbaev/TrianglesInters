#pragma once

#include <iostream>

namespace utils {
  template <typename T>
  struct float_traits {
    static constexpr T kEPS{};
  };

  template <>
  struct float_traits<float> {
    static constexpr float kEPS = 1e-4f;
  };

  template <>
  struct float_traits<double> {
    static constexpr double kEPS = 1e-6;
  };

  template <>
  struct float_traits<long double> {
    static constexpr long double kEPS = 1e-12L;
  };

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

  template <typename T>
  [[nodiscard]] inline signs_t sign(T number) {
    constexpr T kEPS = float_traits<T>::kEPS;
    if (number < -kEPS) {
      return signs_t::NEG;
    }

    return number > kEPS ? signs_t::POS : signs_t::ZERO;
  }

  template<typename T>
  [[nodiscard]] T sqr(T x) { return x * x; }
};
