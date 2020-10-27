#pragma once

#include <base/graph.h>

#include <optional>

struct Literal {
  size_t variable;
  bool negation;
};

using Disjunction = std::pair<Literal, Literal>;

class TwoCNFFormula {
public:
  using Disjunctions = std::vector<Disjunction>;

public:
  explicit TwoCNFFormula(size_t variable_count);

  void AddDisjunction(Literal first_literal, Literal second_literal);
  const Disjunctions& GetDisjunctions() const;
  size_t GetVariableCount() const;

private:
  Disjunctions disjunctions_;
  size_t variable_count_;
};

using SatisfyingAssignment = std::vector<bool>;

class TwoSATSolver {
  using TopologicalOrdering = std::vector<Vertex>;
  using VisitedMask = std::vector<bool>;
  using StronglyConnectedComponents = std::vector<size_t>;

public:
  std::optional<SatisfyingAssignment> GetSatisfyingAssignment(TwoCNFFormula formula);

private:
  DirectedGraph BuildImplicationGraph(TwoCNFFormula formula) const;
  DirectedGraph GetReversedGraph(const DirectedGraph& DirectedGraph) const;

  TopologicalOrdering GetTopologicalOrdering(const DirectedGraph& graph) const;
  void TopologicalOrderingDFS(
      Vertex vertex,
      const DirectedGraph& graph,
      VisitedMask& mask,
      TopologicalOrdering& ordering) const;

  StronglyConnectedComponents BuildStronglyConnectedComponents(const DirectedGraph& graph) const;
  void StronglyConnectedComponentsDFS(
      Vertex vertex,
      const DirectedGraph& graph,
      StronglyConnectedComponents& components,
      size_t current_component_number) const;

  std::optional<SatisfyingAssignment>
  BuildSatisfyingAssignmentFromStronglyConnectedComponents(StronglyConnectedComponents components) const;
};