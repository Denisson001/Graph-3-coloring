#pragma once

#include "graph_coloring_algo.h"

#include <utility/two_SAT_solver.h>

#include <optional>

class Randomized3ColoringAlgo : public Graph3ColoringAlgo {
public:
  using AvailableColors = std::vector<Color>;

public:
  Coloring GetColoring(Graph graph);

private:
  std::optional<Coloring> DoIteration(const Graph& graph);

  std::vector<AvailableColors> GetRandomAvailableColors(size_t vertex_count) const;

  TwoCNFFormula MakeTwoCNFFormula(const Graph& graph, const std::vector<AvailableColors>& available_colors) const;

  Coloring GetColoringFromSatisfyingAssignment(
      const std::vector<AvailableColors>& available_colors,
      const SatisfyingAssignment& satisfying_assignment) const;

private:
  TwoSATSolver two_SAT_solver_;
};