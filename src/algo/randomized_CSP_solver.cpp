#include "randomized_CSP_solver.h"

RandomizedCSPSolver::RandomizedCSPSolver()
    : random_engine_("RandomizedCSPSolver") {}

size_t RandomizedCSPSolver::GetLastRunIterationCount() const {
  return iteration_count_;
}

Coloring RandomizedCSPSolver::GetColoring(const UndirectedGraph& graph) {
  const auto csp = Convert3SATToCSP(graph);
  for (iteration_count_ = 1; ; ++iteration_count_) {
    auto csp_copy = csp;
    const auto csp_solution = TryToSolveSCP(csp_copy);
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

std::optional<CSPSolution> RandomizedCSPSolver::TryToSolveSCP(CSP<3, 2>& csp) {
  if (csp.GetActiveVariables().size() <= 3) {
    return SolveSCPUsingBruteforce(csp);
  }
  if (csp.GetConstraints().size() == 0) {
    return CSPSolution(csp.GetVariableCount(), 1);
  }
  const auto random_constraint = random_engine_.GetRandomValueFrom(csp.GetConstraints());
  const auto first_var_constraint = random_constraint[0];
  const auto second_var_constraint = random_constraint[1];
  CSPSolutionConverters solution_converters;
  solution_converters.AddSolutionConverter(ReassignValuesToVariable(
      csp,
      second_var_constraint.variable,
      second_var_constraint.value,
      first_var_constraint.value));
  const auto forbidden_values = ForbidRandomVariablesValues(
      csp,
      first_var_constraint.variable,
      second_var_constraint.variable,
      first_var_constraint.value);
  solution_converters.AddSolutionConverter(csp.RemoveVariableWithTwoAvailableValues(
      first_var_constraint.variable,
      GetRemainingAvailableValues(forbidden_values.first)));
  solution_converters.AddSolutionConverter(csp.RemoveVariableWithTwoAvailableValues(
      second_var_constraint.variable,
      GetRemainingAvailableValues(forbidden_values.second)));
  auto solution = TryToSolveSCP(csp);
  if (solution.has_value()) {
    return solution_converters.Convert(std::move(*solution));
  }
  return {};
}

template <size_t TColorCount, size_t TConstraintSize>
std::optional<CSPSolution> RandomizedCSPSolver::SolveSCPUsingBruteforce(
    const CSP<TColorCount, TConstraintSize>& csp) const
{
  CSPSolution solution(csp.GetVariableCount(), 1);
  const auto& active_variables = csp.GetActiveVariables();
  while (true) {
    size_t remaining_var_count = active_variables.size();
    for (const auto var : active_variables) {
      ++solution[var];
      --remaining_var_count;
      if (solution[var] == TColorCount + 1) {
        if (remaining_var_count == 0) {
          return {};
        }
        solution[var] = 1;
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

CSPSolutionConverter RandomizedCSPSolver::ReassignValuesToVariable(
    CSP<3, 2>& csp,
    Variable reassigning_variable,
    Value prev_value,
    Value new_value) const
{
  auto value_converter = [prev_value, new_value](Value value) {
    const auto [v1, v2] = GetRemainingAvailableValues(prev_value);
    const auto [c1, c2] = GetRemainingAvailableValues(new_value);
    if (value == prev_value) {
      return new_value;
    }
    if (value == v1) {
      return c1;
    }
    return c2;
  };
  CSPSolutionConverter solution_converter{
      [reassigning_variable, value_converter](Variable var, const CSPSolution &csp_solution) {
        if (var != reassigning_variable) {
          return csp_solution[var];
        }
        return value_converter(csp_solution[var]);
      }
  };
  Constraints constraints = csp.GetConstraintsContain(reassigning_variable);
  csp.RemoveConstraints(constraints);
  for (auto constraint : constraints) {
    for (auto& [var, value] : constraint) {
      if (var == reassigning_variable) {
        value = value_converter(value);
      }
    }
    csp.AddConstraint(constraint);
  }
  return solution_converter;
}

std::pair<Value, Value> RandomizedCSPSolver::ForbidRandomVariablesValues(
    CSP<3, 2>& csp,
    Variable var1,
    Variable var2,
    Value common_value)
{
  const auto remaining_values = GetRemainingAvailableValues(common_value);
  const Value other_forbidden_value = random_engine_.FlipCoin(0.5) ?
      remaining_values.first : remaining_values.second;
  std::pair<Value, Value> forbidden_values{
    common_value,
    other_forbidden_value
  };
  if (random_engine_.FlipCoin(0.5)) {
    std::swap(forbidden_values.first, forbidden_values.second);
  }
  csp.ForbidValueForVariable(var1, forbidden_values.first);
  csp.ForbidValueForVariable(var2, forbidden_values.second);
  return forbidden_values;
}

std::pair<size_t, size_t> RandomizedCSPSolver::GetRemainingAvailableValues(size_t value) {
  switch (value) {
    case 1: return {2, 3};
    case 2: return {1, 3};
    case 3: return {1, 2};
  }
  throw std::logic_error("Wrong variable value");
}