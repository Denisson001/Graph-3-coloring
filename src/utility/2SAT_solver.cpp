#include "2SAT_solver.h"

TwoCNFFormula::TwoCNFFormula(size_t variable_count)
    : variable_count_(variable_count) {}

void TwoCNFFormula::AddDisjunction(Literal first_literal, Literal second_literal) {
  disjunctions_.emplace_back(std::move(first_literal), std::move(second_literal));
}

std::optional<SatisfyingAssignment> TwoSATSolver::GetSatisfyingAssignment(TwoCNFFormula formula) {

}