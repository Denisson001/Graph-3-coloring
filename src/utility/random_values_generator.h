#pragma once

#include "loggable_component.h"

#include <ctime>
#include <random>

class RandomValuesGenerator : private LoggableComponent {
public:
  RandomValuesGenerator(std::string component_name);

  int32_t GenerateRandomValue(int32_t low, int32_t high);
  bool FlipCoin(float coin_probability);

  template<typename T, template<typename> typename Container>
  T GetRandomValueFrom(const Container<T>& container);

private:
  size_t random_seed_;
  std::mt19937_64 random_value_engine_;
};


template<typename T, template<typename> typename Container>
T RandomValuesGenerator::GetRandomValueFrom(const Container<T>& container) {
  if (container.size() == 0) {
    throw std::logic_error("Container is empty");
  }
  size_t index = GenerateRandomValue(0, container.size() - 1);
  auto container_iter = container.begin();
  std::advance(container_iter, index);
  return *container_iter;
}