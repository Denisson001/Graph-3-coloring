#include "benchmark_driver.h"

#include <algo/randomized_algo.h>

#include <unordered_map>
#include <functional>

void BenchmarkDriver::RunBenchmark(const std::string& benchmark_name) {
  static std::unordered_map<std::string, std::function<void()>> benchmarks;
#define addBenchmark(bench_name) benchmarks[#bench_name] = std::bind(&BenchmarkDriver::Run##bench_name, this);
  addBenchmark(RandomizedAlgoBench);
#undef addBenchmark
  benchmarks.at(benchmark_name)();
}

void BenchmarkDriver::RunRandomizedAlgoBench() {
  Randomized3ColoringAlgo algo;
  for (size_t vertex_count = 1; vertex_count <= 30; ++vertex_count) {
    const auto graph = graph_generator_.Generate3ColoringGraph(vertex_count);
    const auto coloring = algo.GetColoring(graph);
    if (!coloring_checker_.CheckColoring(graph, coloring)) {
      throw std::logic_error("Wrong coloring");
    }
  }
}