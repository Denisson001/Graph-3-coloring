#pragma once

#include <math_utility/random_graph_generator.h>
#include <math_utility/graph_coloring_checker.h>
#include <utility/loggable_component.h>

#include <string>

class BenchmarkDriver : private LoggableComponent {
public:
  BenchmarkDriver();

  void RunBenchmark(const std::string& benchmark_name);
  void RunRandomized3ColoringAlgoBenchmark();
  void RunRandomizedCSPSolverBenchmark();

private:
  RandomGraphGenerator graph_generator_;
  GraphColoringChecker coloring_checker_;
};