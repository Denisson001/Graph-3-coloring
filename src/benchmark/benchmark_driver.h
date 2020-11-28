#pragma once

#include <algo/graph_coloring_algo.h>
#include <math_utility/graph_generator.h>
#include <math_utility/graph_coloring_checker.h>
#include <utility/loggable_component.h>

#include <string>

class BenchmarkDriver : private LoggableComponent {
public:
  BenchmarkDriver();

  void RunBenchmark(const std::string &benchmark_name);

  void RunRandomized3ColoringAlgoBenchmark();
  void RunRandomized3ColoringAlgoAllSmallGraphsBenchmark();

  void RunRandomizedCSPSolverBenchmark();
  void RunRandomizedCSPSolverOneGraphIterDistributionBenchmark();

private:
  void RunBenchmark(
      std::string bench_name,
      Graph3ColoringAlgo&& algo,
      size_t iter_count,
      double coin_probability,
      size_t max_vertex_count,
      size_t min_vertex_count = 1);

  void CheckColoring(const UndirectedGraph& graph, std::optional<Coloring> coloring);

private:
  RandomGraphGenerator graph_generator_;
  GraphColoringChecker coloring_checker_;
};