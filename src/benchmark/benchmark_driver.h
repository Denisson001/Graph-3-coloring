#pragma once

#include <utility/random_graph_generator.h>
#include <utility/graph_coloring_checker.h>

#include <string>

class BenchmarkDriver {
public:
  void RunBenchmark(const std::string& benchmark_name);
  void RunRandomizedAlgoBench();

private:
  RandomGraphGenerator graph_generator_;
  GraphColoringChecker coloring_checker_;
};