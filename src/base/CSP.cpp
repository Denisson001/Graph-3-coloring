#include "CSP.h"

bool VariableConstraint::operator<(const VariableConstraint& other_constraint) const {
  return std::make_pair(variable, value) <
      std::make_pair(other_constraint.value, other_constraint.value);
}

bool VariableConstraint::operator==(const VariableConstraint& other_constraint) const {
  return variable == other_constraint.variable &&
         value == other_constraint.value;
}

template<>
void CSP<3, 2>::ForbidValueForVariable(Variable variable, Value value) {
  for (Variable other_var = 0; other_var < variable_count_; ++other_var) {
    for (Value other_value = 1; other_value <= 3; ++other_value) {
      AddConstraint(Constraint{
              VariableConstraint{variable, value},
              VariableConstraint{other_var, other_value}
      });
    }
  }
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
  active_variables_.erase(removing_variable);
  RemoveConstraints(GetConstraintsContain(removing_variable));

  CSPSolutionConverter solution_converter{
      [](Variable var, const CSPSolution &csp_solution) {
        // TODO
        return csp_solution[var];
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