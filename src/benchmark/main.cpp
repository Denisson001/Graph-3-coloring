#include <benchmark/benchmark_driver.h>

int main(int argc, char* argv[]) {
  BenchmarkDriver benchmark_driver;
  benchmark_driver.RunBenchmark(argv[1]);
}