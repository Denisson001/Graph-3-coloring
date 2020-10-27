#pragma once

#include "loggable_component.h"

#include <ctime>
#include <random>

class RandomValuesGenerator : private LoggableComponent {
public:
  RandomValuesGenerator(std::string component_name);
  int32_t GenerateRandomValue(int32_t low, int32_t high);
  bool FlipCoin(float coin_probability);

private:
  size_t random_seed_;
  std::mt19937_64 random_value_engine_;
};
