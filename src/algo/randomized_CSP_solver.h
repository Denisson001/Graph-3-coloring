#pragma once

#include "graph_coloring_algo.h"

#include <base/CSP.h>
#include <utility/random_values_generator.h>

#include <optional>

class RandomizedCSPSolver : public Graph3ColoringAlgo {
  RandomizedCSPSolver();

  Coloring GetColoring(const UndirectedGraph& graph);

  size_t GetLastRunIterationCount() const;

private:
  std::optional<CSPSolution> TryToSolveSCP(const CSP<3, 2>& csp);
  template <size_t TColorCount, size_t TConstraintSize>
  std::optional<CSPSolution> SolveSCPUsingBruteforce(
      const CSP<TColorCount, TConstraintSize>& csp) const;

  void AddRandomExtraConstraints(CSP<3, 2>& csp);

  CSP<3, 2> Convert3SATToCSP(const UndirectedGraph& graph) const;

  std::pair<size_t, size_t> GetRemainingAvailableValues(size_t value) const;

private:
  RandomValuesGenerator random_engine_;
  size_t iteration_count_;
};