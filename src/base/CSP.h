#pragma once

#include <set>
#include <stdexcept>
#include <vector>

using Variable = size_t;
using Value = size_t;

struct VariableConstraint {
public:
  bool operator<(const VariableConstraint& other_constraint) const;

public:
  Variable variable;
  Value value;
};

using Constraint = std::set<VariableConstraint>;
using Constraints = std::set<Constraint>;
using CSPSolution = std::vector<size_t>;

template <size_t TValueCount, size_t TConstraintSize>
class CSP {
public:
  explicit CSP(size_t variable_count)
      : variable_count_(variable_count) {}

  void AddConstraint(Constraint constraint);
  const Constraints& GetConstraints() const;
  Constraints GetConstraintsContain(Variable variable) const;

  void RemoveConstraints(const Constraints& constraints);

  size_t GetVariableCount() const;

  void ForbidValueForVariable(Variable variable, Value value);

  bool CheckSolution(const CSPSolution& solution) const;

private:
  bool ConstraintContainsVariable(
      const Constraint& constraint,
      Variable variable) const;

private:
  size_t variable_count_;
  Constraints constraints_;
};


template <size_t TValueCount, size_t TConstraintSize>
void CSP<TValueCount, TConstraintSize>::AddConstraint(Constraint constraint) {
  if (constraint.size() != TConstraintSize) {
    throw std::logic_error("Wrong constraint size");
  }
  constraints_.insert(std::move(constraint));
}

template <size_t TValueCount, size_t TConstraintSize>
const Constraints& CSP<TValueCount, TConstraintSize>::GetConstraints() const {
  return constraints_;
}

template <size_t TValueCount, size_t TConstraintSize>
Constraints CSP<TValueCount, TConstraintSize>::GetConstraintsContain(Variable variable) const {
  Constraints constraints;
  for (const auto& constraint : constraints_) {
    if (ConstraintContainsVariable(constraint, variable)) {
      constraints.insert(constraint);
    }
  }
  return constraints;
}

template <size_t TValueCount, size_t TConstraintSize>
void CSP<TValueCount, TConstraintSize>::RemoveConstraints(const Constraints& constraints) {
  for (const auto& constraint : constraints) {
    constraints_.erase(constraint);
  }
}

template <size_t TValueCount, size_t TConstraintSize>
size_t CSP<TValueCount, TConstraintSize>::GetVariableCount() const {
  return variable_count_;
}

template <size_t TValueCount, size_t TConstraintSize>
bool CSP<TValueCount, TConstraintSize>::CheckSolution(const CSPSolution& solution) const {
  if (solution.size() != variable_count_) {
    return false;
  }
  for (auto value : solution) {
    if (value < 1 || value > TValueCount) {
      return false;
    }
  }
  for (const auto& constraint : constraints_) {
    bool satisfied = false;
    for (const auto& [variable, value] : constraint) {
      if (solution[variable] != value) {
        satisfied = true;
        break;
      }
    }
    if (!satisfied) {
      return false;
    }
  }
  return true;
}

template <size_t TValueCount, size_t TConstraintSize>
bool CSP<TValueCount, TConstraintSize>::ConstraintContainsVariable(
    const Constraint& constraint,
    Variable variable) const
{
  for (Value value = 1; value <= TValueCount; ++value) {
    if (constraint.find(VariableConstraint{variable, value}) != constraint.end()) {
      return true;
    }
  }
  return false;
}