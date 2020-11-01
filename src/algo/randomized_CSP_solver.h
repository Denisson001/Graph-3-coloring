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
  std::optional<CSPSolution> TryToSolveSCP(CSP<3, 2>& csp);
  template <size_t TColorCount, size_t TConstraintSize>
  std::optional<CSPSolution> SolveSCPUsingBruteforce(
      const CSP<TColorCount, TConstraintSize>& csp) const;

  CSPSolutionConverter ReassignValuesToVariable(
      CSP<3, 2>& csp,
      Variable reassigning_variable,
      Value prev_value,
      Value new_value) const;
  std::pair<Value, Value> ForbidRandomVariablesValues(
      CSP<3, 2>& csp,
      Variable var1,
      Variable var2,
      Value common_value);

  CSP<3, 2> Convert3SATToCSP(const UndirectedGraph& graph) const;

  static std::pair<size_t, size_t> GetRemainingAvailableValues(size_t value);

private:
  RandomValuesGenerator random_engine_;
  size_t iteration_count_;
};