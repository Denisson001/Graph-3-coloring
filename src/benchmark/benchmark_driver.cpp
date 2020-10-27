#include "benchmark_driver.h"

#include <algo/randomized_algo.h>
#include <utility/csv_writer.h>

#include <functional>
#include <unordered_map>

BenchmarkDriver::BenchmarkDriver()
    : LoggableComponent("BenchmarkDriver") {}

void BenchmarkDriver::RunBenchmark(const std::string& benchmark_name) {
  static std::unordered_map<std::string, std::function<void()>> benchmarks;
#define addBenchmark(bench_name) benchmarks[#bench_name] = std::bind(&BenchmarkDriver::Run##bench_name, this);
  addBenchmark(RandomizedAlgoBenchmark);
#undef addBenchmark
  PrintLogMessage("Start " + benchmark_name);
  benchmarks.at(benchmark_name)();
  PrintLogMessage(benchmark_name + " finished");
}

void BenchmarkDriver::RunRandomizedAlgoBenchmark() {
  Randomized3ColoringAlgo algo;
  CSVWriter csv_writer{"RandomizedAlgoBenchmark"};
  for (size_t iter = 1; iter <= 1; ++iter) {
    PrintLogMessage("Start iteration " + std::to_string(iter) + "/30");
    for (size_t vertex_count = 1; vertex_count <= 30; ++vertex_count) {
      const auto graph = graph_generator_.Generate3ColoringGraph(vertex_count);
      const auto coloring = algo.GetColoring(graph);
      if (!coloring_checker_.CheckColoring(graph, coloring)) {
        throw std::logic_error("Wrong coloring");
      }
      csv_writer.AddValue(std::to_string(algo.GetLastRunIterationCount()));
    }
    csv_writer.FlushValues();
  }
}