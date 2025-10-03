#pragma once

namespace utils {
  inline const float       kEPS_f  = 1e-4;
  inline const double      kEPS_d  = 1e-6;
  inline const long double kEPS_ld = 1e-10;

  [[nodiscard]] int sign(float x) {
    return x < -kEPS_f ? -1 : static_cast<int>(x > kEPS_f);
  }

  [[nodiscard]] int sign(double x) {
    return x < -kEPS_d ? -1 : static_cast<int>(x > kEPS_d);
  }

  [[nodiscard]] int sign(long double x) {
    return x < -kEPS_ld ? -1 : static_cast<int>(x > kEPS_ld);
  }

  template<typename T>
  [[nodiscard]] T sqr(T x) { return x * x; }
};
