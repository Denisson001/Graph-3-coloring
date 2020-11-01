#include "CSP.h"

bool VariableConstraint::operator<(const VariableConstraint& other_constraint) const {
  return std::make_pair(variable, value) <
      std::make_pair(other_constraint.value, other_constraint.value);
}

bool VariableConstraint::operator==(const VariableConstraint& other_constraint) const {
  return variable == other_constraint.variable &&
         value == other_constraint.value;
}

template <>
CSPSolutionConverter CSP<3, 2>::RemoveVariableWithTwoAvailableValues(
    Variable removing_variable,
    std::pair<Value, Value> available_values)
{
  const auto first_constraints =
      GetConstraintsContain(VariableConstraint{removing_variable, available_values.first});
  const auto second_constraints =
      GetConstraintsContain(VariableConstraint{removing_variable, available_values.second});

  auto extract_remaining_constraint = [removing_variable](const Constraint& constraint) {
    for (const auto& var_constraint : constraint) {
      if (var_constraint.variable != removing_variable) {
        return var_constraint;
      }
    }
  };

  auto do_cartesian_product = [extract_remaining_constraint](
      const Constraints& first_constraints,
      const Constraints& second_constraints)
  {
    Constraints constraints;
    for (const auto& first_constraint : first_constraints) {
      for (const auto& second_constraint : second_constraints) {
        constraints.insert(Constraint{
            extract_remaining_constraint(first_constraint),
            extract_remaining_constraint(second_constraint)
        });
      }
    }
    return constraints;
  };

  AddConstraints(do_cartesian_product(first_constraints, second_constraints));
  const auto removing_variable_constraints = GetConstraintsContain(removing_variable);
  RemoveConstraints(removing_variable_constraints);
  active_variables_.erase(removing_variable);

  CSPSolutionConverter solution_converter{
      [removing_variable, available_values, removing_variable_constraints, extract_remaining_constraint](
          Variable var,
          const CSPSolution &csp_solution)
      {
        if (var != removing_variable) {
          return csp_solution[var];
        }
        for (const auto& possible_value : {available_values.first, available_values.second}) {
          bool satisfies = true;
          for (const auto& constraint : removing_variable_constraints) {
            const auto remaining_variable_constraint = extract_remaining_constraint(constraint);
            if (csp_solution[remaining_variable_constraint.variable] == remaining_variable_constraint.value) {
              satisfies = false;
              break;
            }
          }
          if (satisfies) {
            return possible_value;
          }
        }
      }
  };

  return solution_converter;
}

CSPSolutionConverter::CSPSolutionConverter(VariableValueConverter variable_value_converter)
    : variable_value_converter_(variable_value_converter) {}

CSPSolution CSPSolutionConverter::Convert(CSPSolution&& solution) const {
  for (Variable var = 0; var < solution.size(); ++var) {
    solution[var] = variable_value_converter_(var, solution);
  }
  return solution;
}

void CSPSolutionConverters::AddSolutionConverter(CSPSolutionConverter solution_converter) {
  solution_converters_.emplace_front(std::move(solution_converter));
}

CSPSolution CSPSolutionConverters::Convert(CSPSolution&& solution) const {
  for (const auto& solution_converter : solution_converters_) {
    solution = solution_converter.Convert(std::move(solution));
  }
  return solution;
}