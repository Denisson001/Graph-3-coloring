#include "benchmark_driver.h"

#include <algo/randomized_3coloring_algo.h>
#include <algo/randomized_CSP_solver.h>
#include <utility/csv_writer.h>

#include <functional>
#include <unordered_map>

BenchmarkDriver::BenchmarkDriver()
    : LoggableComponent("BenchmarkDriver") {}

void BenchmarkDriver::RunBenchmark(const std::string& benchmark_name) {
  static std::unordered_map<std::string, std::function<void()>> benchmarks;
#define addBenchmark(bench_name) benchmarks[#bench_name] = std::bind(&BenchmarkDriver::Run##bench_name, this);
  addBenchmark(Randomized3ColoringAlgoBenchmark);
  addBenchmark(RandomizedCSPSolverBenchmark);
  addBenchmark(Randomized3ColoringAlgoAllSmallGraphsBenchmark);
  addBenchmark(RandomizedCSPSolverOneGraphIterDistributionBenchmark);
#undef addBenchmark
  PrintLogMessage("Start " + benchmark_name);
  benchmarks.at(benchmark_name)();
  PrintLogMessage(benchmark_name + " finished");
}

void BenchmarkDriver::RunRandomized3ColoringAlgoBenchmark() {
  RunBenchmark("Randomized3ColoringAlgoBenchmark", Randomized3ColoringAlgo(), 50, 0.7, 30);
}

void BenchmarkDriver::RunRandomizedCSPSolverBenchmark() {
  RunBenchmark("RandomizedCSPSolverBenchmark", RandomizedCSPSolver(), 20, 0.125, 80);
}

void BenchmarkDriver::RunRandomizedCSPSolverOneGraphIterDistributionBenchmark() {
  RunBenchmark("RandomizedCSPSolverOneGraphIterDistributionBenchmark", RandomizedCSPSolver(), 2000, 0.15, 50, 50);
}

void BenchmarkDriver::RunBenchmark(
    std::string bench_name,
    Graph3ColoringAlgo&& algo,
    size_t iter_count,
    double coin_probability,
    size_t max_vertex_count,
    size_t min_vertex_count)
{
  CSVWriter csv_writer{bench_name + ".p=" + std::to_string(coin_probability)};
  for (size_t iter = 1; iter <= iter_count; ++iter) {
    PrintLogMessage("Start iteration " + std::to_string(iter) +
                    "/" + std::to_string(iter_count) +
                    " p=" + std::to_string(coin_probability));
    for (size_t vertex_count = min_vertex_count; vertex_count <= max_vertex_count; ++vertex_count) {
      const auto graph = graph_generator_.Generate3ColoringGraph(vertex_count, coin_probability);
      const auto coloring = algo.GetColoring(graph);
      CheckColoring(graph, coloring);
      csv_writer.AddValue(std::to_string(algo.GetLastRunIterationCount()));
      PrintLogMessage("Coloring was found: vertex_count: " + std::to_string(vertex_count) +
                      " algo_iterations: " + std::to_string(algo.GetLastRunIterationCount()));
    }
    csv_writer.FlushValues();
  }
}

void BenchmarkDriver::RunRandomized3ColoringAlgoAllSmallGraphsBenchmark() {
  const size_t vertex_count = 5;
  const size_t iter_count = 100;
  CSVWriter csv_writer{"Randomized3ColoringAlgoAllSmallGraphsBenchmark4"
                       ".vertex_count=" + std::to_string(vertex_count)};
  GraphGenerator graph_generator;
  const auto graphs = graph_generator.GenerateAllGraphs(vertex_count);
  auto algo = Randomized3ColoringAlgo(30);

  for (size_t iter = 1; iter <= iter_count; ++iter) {
    PrintLogMessage("Start iteration " + std::to_string(iter) +
                    "/" + std::to_string(iter_count) +
                    " vertex_count=" + std::to_string(vertex_count));
    for (const auto& graph : graphs) {
      const auto coloring = algo.GetColoring(graph);
      if (!coloring.has_value()) {
        continue;
      }
      CheckColoring(graph, coloring);
      csv_writer.AddValue(std::to_string(algo.GetLastRunIterationCount()));
      csv_writer.FlushValues();
    }
  }
}

void BenchmarkDriver::CheckColoring(const UndirectedGraph& graph, std::optional<Coloring> coloring) {
  if (!coloring.has_value()) {
    throw std::logic_error("Coloring wasn't found");
  }
  if (!coloring_checker_.CheckColoring(graph, *coloring)) {
    throw std::logic_error("Wrong coloring");
  }
}