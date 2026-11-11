#pragma once
#include <random>
namespace Tnk {

class Random {
 public:
  Random(const Random&) = delete;
  Random& operator=(const Random&) = delete;

  static std::mt19937& get_engine() {
    thread_local static std::mt19937 engine([]() {
      std::random_device rd;
      return std::mt19937(rd());
    }());
    return engine;
  }

  template <typename T>
  static T getInt(T min, T max) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(get_engine());
  }

  template <typename T>
  static T getReal(T min, T max) {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(get_engine());
  }
};

}  // namespace Tnk
