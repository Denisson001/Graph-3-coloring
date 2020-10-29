#include "randomized_CSP_solver.h"

RandomizedCSPSolver::RandomizedCSPSolver()
    : random_engine_("RandomizedCSPSolver") {}

size_t RandomizedCSPSolver::GetLastRunIterationCount() const {
  return iteration_count_;
}

Coloring RandomizedCSPSolver::GetColoring(const UndirectedGraph& graph) {
  const auto csp = Convert3SATToCSP(graph);
  for (iteration_count_ = 1; ; ++iteration_count_) {
    const auto csp_solution = TryToSolveSCP(csp);
    if (csp_solution.has_value()) {
      return static_cast<Coloring>(*csp_solution);
    }
  }
}

CSP<3, 2> RandomizedCSPSolver::Convert3SATToCSP(const UndirectedGraph& graph) const {
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

std::optional<CSPSolution> RandomizedCSPSolver::TryToSolveSCP(const CSP<3, 2>& csp) {
  if (csp.GetVariableCount() <= 3) {
    return SolveSCPUsingBruteforce(csp);
  }
  if (csp.GetConstraints().size() == 0) {
    return CSPSolution(csp.GetVariableCount(), 1);
  }
  const size_t constraint_index =
      random_engine_.GenerateRandomValue(0, csp.GetConstraints().size() - 1);

}

template <size_t TColorCount, size_t TConstraintSize>
std::optional<CSPSolution> RandomizedCSPSolver::SolveSCPUsingBruteforce(
    const CSP<TColorCount, TConstraintSize>& csp) const
{
  CSPSolution solution(csp.GetVariableCount(), 1);
  while (true) {
    for (size_t index = 0; index < solution.size(); ++index) {
      ++solution[index];
      if (solution[index] == TColorCount + 1) {
        if (index + 1 == solution.size()) {
          return {};
        }
        solution[index] = 1;
      } else {
        break;
      }
    }
    if (csp.CheckSolution(solution)) {
      return solution;
    }
  }
  return {};
}

void RandomizedCSPSolver::AddRandomExtraConstraints(CSP<3, 2>& csp) {
  // TODO
}

std::pair<size_t, size_t> RandomizedCSPSolver::GetRemainingAvailableValues(size_t value) const {
  switch (value) {
    case 1: return {2, 3};
    case 2: return {1, 3};
    case 3: return {1, 2};
  }
  throw std::logic_error("Wrong value");
}