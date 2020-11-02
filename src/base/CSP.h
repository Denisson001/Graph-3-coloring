#pragma once

#include <algorithm>
#include <deque>
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

class CSPSolutionConverter {
public:
  using VariableValueConverter = std::function<Value(Variable, const CSPSolution&)>;

public:
  explicit CSPSolutionConverter(VariableValueConverter variable_value_converter);

  CSPSolution Convert(CSPSolution&& solution) const;

private:
  VariableValueConverter variable_value_converter_;
};

class CSPSolutionConverters {
public:
  void AddSolutionConverter(CSPSolutionConverter solution_converter);
  CSPSolution Convert(CSPSolution&& solution) const;

private:
  std::deque<CSPSolutionConverter> solution_converters_;
};

template <size_t TValueCount, size_t TConstraintSize>
class CSP {
public:
  using Variables = std::set<Variable>;

public:
  explicit CSP(size_t variable_count);

  void AddConstraint(Constraint constraint);
  void AddConstraints(Constraints constraints);

  void RemoveConstraints(const Constraints& constraints);

  const Constraints& GetConstraints() const;
  Constraints GetConstraintsContain(Variable variable) const;
  Constraints GetConstraintsContain(VariableConstraint variable_constraint) const;

  const Variables& GetActiveVariables() const;
  size_t GetVariableCount() const;

  void ForbidVariableValue(Variable variable, Value value);

  CSPSolutionConverter ApplyEquivalentTransformTo(Variable removing_variable);
  CSPSolutionConverter RemoveVariableWithOneAvailableValue(
      Variable removing_variable,
      Value available_value);
  CSPSolutionConverter RemoveVariableWithTwoAvailableValues(
      Variable removing_variable,
      std::pair<Value, Value> available_values);

  std::vector<Value> GetAvailableValuesOf(Variable variable) const;
  bool AllVariablesHaveAvailableValue() const;

  bool CheckSolution(const CSPSolution& solution) const;

private:
  bool ConstraintContainsVariable(
      const Constraint& constraint,
      Variable variable) const;

  bool ConstraintContainsVariableConstraint(
      const Constraint& constraint,
      VariableConstraint variable_constraint) const;

private:
  Variables active_variables_;
  Constraints constraints_;
  size_t variable_count_;
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
void CSP<TValueCount, TConstraintSize>::AddConstraints(Constraints constraints) {
  for (auto& constraint : constraints) {
    AddConstraint(std::move(constraint));
  }
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
Constraints CSP<TValueCount, TConstraintSize>::GetConstraintsContain(
    VariableConstraint variable_constraint) const
{
  Constraints constraints;
  for (const auto& constraint : constraints_) {
    if (ConstraintContainsVariableConstraint(constraint, variable_constraint)) {
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
void CSP<TValueCount, TConstraintSize>::ForbidVariableValue(Variable variable, Value value) {
  AddConstraint(Constraint(TConstraintSize, VariableConstraint{variable, value}));
}


template <size_t TValueCount, size_t TConstraintSize>
std::vector<Value> CSP<TValueCount, TConstraintSize>::GetAvailableValuesOf(Variable variable) const {
  std::vector<Value> available_values;
  for (Value value = 1; value <= TValueCount; ++value) {
    bool is_available = true;
    for (const auto &constraint : constraints_) {
      if (constraint == Constraint(TConstraintSize, VariableConstraint{variable, value})) {
        is_available = false;
        break;
      }
    }
    if (is_available) {
      available_values.push_back(value);
    }
  }
  return available_values;
}

template <size_t TValueCount, size_t TConstraintSize>
bool CSP<TValueCount, TConstraintSize>::AllVariablesHaveAvailableValue() const {
  for (const auto var : active_variables_) {
    if (GetAvailableValuesOf(var).empty()) {
      return false;
    }
  }
  return true;
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
    if (ConstraintContainsVariableConstraint(constraint, VariableConstraint{variable, value})) {
      return true;
    }
  }
  return false;
}

template <size_t TValueCount, size_t TConstraintSize>
bool CSP<TValueCount, TConstraintSize>::ConstraintContainsVariableConstraint(
    const Constraint& constraint,
    VariableConstraint variable_constraint) const
{
  const auto iter = std::find(constraint.begin(), constraint.end(), variable_constraint);
  return iter != constraint.end();
}