#include "randomized_CSP_algo.h"

RandomizedCSPAlgo::RandomizedCSPAlgo()
    : random_engine_("RandomizedCSPAlgo") {}

size_t RandomizedCSPAlgo::GetLastRunIterationCount() const {
  return iteration_count_;
}

Coloring RandomizedCSPAlgo::GetColoring(const UndirectedGraph& graph) {
  const auto csp = Convert3SATToCSP(graph);
  for (iteration_count_ = 1; ; ++iteration_count_) {
    const auto csp_solution = DoIteration(csp);
    if (csp_solution.has_value()) {
      return ConvertCSPSolutionTo3Coloring(std::move(*csp_solution));
    }
  }
}

CSP<3, 2> RandomizedCSPAlgo::Convert3SATToCSP(const UndirectedGraph& graph) const {
  CSP<3, 2> csp(graph.GetVertexCount());
  for (const auto& [u, v] : graph.GetEdges()) {
    for (Color color = 1; color <= 3; ++color) {
      csp.AddConstraint(Constraint{
          VariableConstraint{u, color},
          VariableConstraint{v, color}
      });
    }
  }
  return csp;
}

std::optional<CSPSolution> RandomizedCSPAlgo::DoIteration(const CSP<3, 2>& csp) {
  // TODO
}

Coloring RandomizedCSPAlgo::ConvertCSPSolutionTo3Coloring(CSPSolution csp_solution) const {
  // TODO
}