#include "random_values_generator.h"

RandomValuesGenerator::RandomValuesGenerator(std::string component_name)
    : LoggableComponent(component_name + ":RandomValuesGenerator"),
      random_seed_(std::time(nullptr) + rand()),
      random_value_engine_(random_seed_)
{
  PrintLogMessage("Started with seed: " + std::to_string(random_seed_));
}

int32_t RandomValuesGenerator::GenerateRandomValue(int32_t low, int32_t high) {
  std::uniform_int_distribution<int32_t> generator(low, high);
  return generator(random_value_engine_);
}

bool RandomValuesGenerator::FlipCoin(float coin_probability) {
  static const int32_t precision = 1000000;
  return GenerateRandomValue(1, precision) <= coin_probability * precision;
}