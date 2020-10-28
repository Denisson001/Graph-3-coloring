#pragma once

#include <stdexcept>
#include <vector>

using Variable = size_t;
using Value = size_t;

struct VariableConstraint {
  Variable variable;
  Value value;
};

using Constraint = std::vector<VariableConstraint>;
using Constraints = std::vector<Constraint>;
using CSPSolution = std::vector<size_t>;

template <size_t TColorCount, size_t TConstraintSize>
class CSP {
public:
  explicit CSP(size_t variable_count)
      : variable_count_(variable_count) {}

  void AddConstraint(Constraint constraint);
  const Constraints& GetConstraints() const;

  bool CheckSolution(const CSPSolution& solution) const;

private:
  size_t variable_count_;
  Constraints constraints_;
};


template <size_t TColorCount, size_t TConstraintSize>
void CSP<TColorCount, TConstraintSize>::AddConstraint(Constraint constraint) {
  if (constraint.size() != TConstraintSize) {
    throw std::logic_error("Wrong constraint size");
  }
  constraints_.emplace_back(std::move(constraint));
}

template <size_t TColorCount, size_t TConstraintSize>
const Constraints& CSP<TColorCount, TConstraintSize>::GetConstraints() const {
  return constraints_;
}

template <size_t TColorCount, size_t TConstraintSize>
bool CSP<TColorCount, TConstraintSize>::CheckSolution(const CSPSolution& solution) const {
  if (solution.size() != variable_count_) {
    return false;
  }
  for (auto value : solution) {
    if (value < 1 || value > TColorCount) {
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