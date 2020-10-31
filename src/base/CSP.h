#pragma once

#include <algorithm>
#include <functional>
#include <set>
#include <stdexcept>
#include <vector>

using Variable = size_t;
using Value = size_t;

struct VariableConstraint {
public:
  bool operator<(const VariableConstraint& other_constraint) const;
  bool operator==(const VariableConstraint& other_constraint) const;

public:
  Variable variable;
  Value value;
};

using Constraint = std::vector<VariableConstraint>;
using Constraints = std::set<Constraint>;
using CSPSolution = std::vector<size_t>;

template <size_t TValueCount, size_t TConstraintSize>
class CSP {
public:
  using Variables = std::set<Variable>;

public:
  explicit CSP(size_t variable_count);

  void AddConstraint(Constraint constraint);
  const Constraints& GetConstraints() const;
  Constraints GetConstraintsContain(Variable variable) const;
  const Variables& GetActiveVariables() const;
  size_t GetVariableCount() const;

  void RemoveConstraints(const Constraints& constraints);

  void ForbidValueForVariable(Variable variable, Value value);

  bool CheckSolution(const CSPSolution& solution) const;

private:
  bool ConstraintContainsVariable(
      const Constraint& constraint,
      Variable variable) const;

private:
  Variables active_variables_;
  Constraints constraints_;
  size_t variable_count_;
};

class CSPSolutionConverter {
public:
  using VariableValueConverter = std::function<Value(Variable, const CSPSolution&)>;

public:
  explicit CSPSolutionConverter(VariableValueConverter variable_value_converter);

  CSPSolution Convert(CSPSolution&& solution) const;

private:
  VariableValueConverter variable_value_converter_;
};


template <size_t TValueCount, size_t TConstraintSize>
CSP<TValueCount, TConstraintSize>::CSP(size_t variable_count)
    : variable_count_(variable_count)
{
  for (Variable var = 0; var < variable_count; ++var) {
    active_variables_.insert(var);
  }
}

template <size_t TValueCount, size_t TConstraintSize>
void CSP<TValueCount, TConstraintSize>::AddConstraint(Constraint constraint) {
  if (constraint.size() != TConstraintSize) {
    throw std::logic_error("Wrong constraint size");
  }
  std::sort(constraint.begin(), constraint.end());
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
const typename CSP<TValueCount, TConstraintSize>::Variables&
CSP<TValueCount, TConstraintSize>::GetActiveVariables() const {
  return active_variables_;
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
    const auto iter = std::find(constraint.begin(), constraint.end(), VariableConstraint{variable, value});
    if (iter != constraint.end()) {
      return true;
    }
  }
  return false;
}