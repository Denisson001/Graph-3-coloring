#pragma once

#include <base/graph.h>

#include <optional>

struct Literal {
  size_t variable;
  bool negation;
};

class TwoCNFFormula {
public:
  using Disjunction = std::pair<Literal, Literal>;

public:
  explicit TwoCNFFormula(size_t variable_count);

  void AddDisjunction(Literal first_literal, Literal second_literal);

private:
  std::vector<Disjunction> disjunctions_;
  size_t variable_count_;
};

using SatisfyingAssignment = std::vector<bool>;

class TwoSATSolver {
public:
  std::optional<SatisfyingAssignment> GetSatisfyingAssignment(TwoCNFFormula formula);
};