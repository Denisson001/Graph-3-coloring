#pragma once

#include <functional>
#include <chrono>

template<typename T>
struct MeasurementResult {
  MeasurementResult(const T& value)
      : value(value) {}

  MeasurementResult(T&& value)
      : value(std::move(value)) {}

  T value;
  std::chrono::milliseconds elapsed_time;
};

class Timer {
public:
  using Task = std::function<T()>;

  template<typename T>
  MeasurementResult<T> MeasureElapsedTime(Task task) const {
    const auto start = std::chrono::high_resolution_clock::now();
    MeasurementResult<T> measurement_result = task();
    const auto finish = std::chrono::high_resolution_clock::now();
    measurement_result.elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    return measurement_result;
  }
};
