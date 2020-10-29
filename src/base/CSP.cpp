#include "CSP.h"

bool VariableConstraint::operator<(const VariableConstraint& other_constraint) const {
  return std::make_pair(variable, value) <
      std::make_pair(other_constraint.value, other_constraint.value);
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