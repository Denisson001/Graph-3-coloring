#pragma once

#include "graph_coloring_algo.h"

#include <math_utility/2SAT_solver.h>
#include <utility/random_values_generator.h>

#include <optional>

class Randomized3ColoringAlgo : public Graph3ColoringAlgo {
  using AvailableColors = std::vector<Color>;
  const static size_t kMaxIterationCount = 10000000;

public:
  Randomized3ColoringAlgo(size_t max_iteration_count = kMaxIterationCount);

  std::optional<Coloring> GetColoring(const UndirectedGraph& graph);

  size_t GetLastRunIterationCount() const;

private:
  std::optional<Coloring> DoIteration(const UndirectedGraph& graph);

  std::vector<AvailableColors> GetRandomAvailableColors(size_t vertex_count);

  TwoCNFFormula MakeTwoCNFFormula(
      const UndirectedGraph& graph,
      const std::vector<AvailableColors>& available_colors) const;

  Coloring GetColoringFromSatisfyingAssignment(
      const std::vector<AvailableColors>& available_colors,
      const SatisfyingAssignment& satisfying_assignment) const;

private:
  TwoSATSolver twoSAT_solver_;
  RandomValuesGenerator random_engine_;
  size_t iteration_count_;
  size_t max_iteration_count_;
};