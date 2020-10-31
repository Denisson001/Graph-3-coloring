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

CSPSolutionConverter::CSPSolutionConverter(VariableValueConverter variable_value_converter)
    : variable_value_converter_(variable_value_converter) {}

CSPSolution CSPSolutionConverter::Convert(CSPSolution&& solution) const {
  for (Variable var = 0; var < solution.size(); ++var) {
    solution[var] = variable_value_converter_(var, solution);
  }
  return solution;
}