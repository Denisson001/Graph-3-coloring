#pragma once

#include "graph_coloring_algo.h"

#include <base/CSP.h>
#include <utility/random_values_generator.h>

#include <optional>

class RandomizedCSPAlgo : public Graph3ColoringAlgo {
  RandomizedCSPAlgo();

  Coloring GetColoring(const UndirectedGraph& graph);

  size_t GetLastRunIterationCount() const;

private:
  std::optional<CSPSolution> DoIteration(const CSP<3, 2>& csp);

  Coloring ConvertCSPSolutionTo3Coloring(CSPSolution csp_solution) const;
  CSP<3, 2> Convert3SATToCSP(const UndirectedGraph& graph) const;

private:
  RandomValuesGenerator random_engine_;
  size_t iteration_count_;
};