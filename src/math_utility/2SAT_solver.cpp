#include "2SAT_solver.h"

#include <algorithm>

TwoCNFFormula::TwoCNFFormula(size_t variable_count)
    : variable_count_(variable_count) {}

void TwoCNFFormula::AddDisjunction(Literal first_literal, Literal second_literal) {
  disjunctions_.emplace_back(std::move(first_literal), std::move(second_literal));
}

const TwoCNFFormula::Disjunctions& TwoCNFFormula::GetDisjunctions() const {
  return disjunctions_;
}

size_t TwoCNFFormula::GetVariableCount() const {
  return variable_count_;
}

std::optional<SatisfyingAssignment> TwoSATSolver::GetSatisfyingAssignment(TwoCNFFormula formula) {
  const auto implication_graph = BuildImplicationGraph(std::move(formula));
  const auto strongly_connected_components = BuildStronglyConnectedComponents(implication_graph);
  return BuildSatisfyingAssignmentFromStronglyConnectedComponents(std::move(strongly_connected_components));
}

DirectedGraph TwoSATSolver::BuildImplicationGraph(TwoCNFFormula formula) const {
  DirectedGraph implication_graph{formula.GetVariableCount() * 2};
  auto convert_literal_to_vertex = [](Literal literal) {
    return literal.variable * 2 + literal.negation;
  };
  for (const auto& disjunction : formula.GetDisjunctions()) {
    const auto u = convert_literal_to_vertex(disjunction.first);
    const auto v = convert_literal_to_vertex(disjunction.second);
    implication_graph.AddEdge(Edge{u ^ 1, v});
    implication_graph.AddEdge(Edge{v ^ 1, u});
  }
  return implication_graph;
}

DirectedGraph TwoSATSolver::GetReversedGraph(const DirectedGraph& graph) const {
  DirectedGraph reversed_graph{graph.GetVertexCount()};
  for (const auto& [u, v] : graph.GetEdges()) {
    reversed_graph.AddEdge(Edge{v, u});
  }
  return reversed_graph;
}

TwoSATSolver::StronglyConnectedComponents
TwoSATSolver::BuildStronglyConnectedComponents(const DirectedGraph& graph) const {
  const auto topological_ordering = GetTopologicalOrdering(graph);
  const auto reversed_graph = GetReversedGraph(graph);
  StronglyConnectedComponents components(graph.GetVertexCount(), 0);
  size_t components_count = 0;
  for (const auto vertex : topological_ordering) {
    if (!components[vertex]) {
      StronglyConnectedComponentsDFS(vertex, reversed_graph, components, ++components_count);
    }
  }
  return components;
}

void TwoSATSolver::StronglyConnectedComponentsDFS(
    Vertex vertex,
    const DirectedGraph& graph,
    StronglyConnectedComponents& components,
    size_t current_component_number) const
{
  components[vertex] = current_component_number;
  for (const auto next_vertex : graph.GetAdjacentVertices(vertex)) {
    if (!components[next_vertex]) {
      StronglyConnectedComponentsDFS(next_vertex, graph, components, current_component_number);
    }
  }
}

TwoSATSolver::TopologicalOrdering TwoSATSolver::GetTopologicalOrdering(const DirectedGraph& graph) const {
  TopologicalOrdering ordering;
  VisitedMask mask(graph.GetVertexCount(), false);
  for (Vertex vertex = 0; vertex < graph.GetVertexCount(); ++vertex) {
    if (!mask[vertex]) {
      TopologicalOrderingDFS(vertex, graph, mask, ordering);
    }
  }
  std::reverse(ordering.begin(), ordering.end());
  return ordering;
}

void TwoSATSolver::TopologicalOrderingDFS(
    Vertex vertex,
    const DirectedGraph& graph,
    VisitedMask& mask,
    TopologicalOrdering& ordering) const
{
  mask[vertex] = true;
  for (const auto next_vertex : graph.GetAdjacentVertices(vertex)) {
    if (!mask[next_vertex]) {
      TopologicalOrderingDFS(next_vertex, graph, mask, ordering);
    }
  }
  ordering.emplace_back(vertex);
}

std::optional<SatisfyingAssignment>
TwoSATSolver::BuildSatisfyingAssignmentFromStronglyConnectedComponents(
    StronglyConnectedComponents components) const
{
  for (Vertex vertex = 0; vertex < components.size(); ++vertex) {
    if (components[vertex] == components[vertex ^ 1]) {
      return {};
    }
  }
  SatisfyingAssignment satisfying_assignment;
  for (Vertex vertex = 0; vertex < components.size(); vertex += 2) {
    satisfying_assignment.emplace_back(components[vertex] > components[vertex ^ 1]);
  }
  return satisfying_assignment;
}