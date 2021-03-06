#include "randomized_3coloring_algo.h"

Randomized3ColoringAlgo::Randomized3ColoringAlgo(size_t max_iter_count)
    : random_engine_("Randomized3ColoringAlgo"),
      max_iteration_count_(max_iter_count) {}

size_t Randomized3ColoringAlgo::GetLastRunIterationCount() const {
  return iteration_count_;
}

std::optional<Coloring> Randomized3ColoringAlgo::GetColoring(const UndirectedGraph& graph) {
  for (iteration_count_ = 1; iteration_count_ <= max_iteration_count_; ++iteration_count_) {
    const auto coloring = DoIteration(graph);
    if (coloring.has_value()) {
      return coloring;
    }
  }
  return {};
}

std::optional<Coloring> Randomized3ColoringAlgo::DoIteration(const UndirectedGraph& graph) {
  const auto available_colors = GetRandomAvailableColors(graph.GetVertexCount());
  const auto two_cnf_formula = MakeTwoCNFFormula(graph, available_colors);
  const auto satisfying_assignment = twoSAT_solver_.GetSatisfyingAssignment(std::move(two_cnf_formula));
  if (!satisfying_assignment.has_value()) {
    return {};
  }
  return GetColoringFromSatisfyingAssignment(available_colors, *satisfying_assignment);
}

TwoCNFFormula Randomized3ColoringAlgo::MakeTwoCNFFormula(
    const UndirectedGraph& graph,
    const std::vector<AvailableColors>& available_colors) const
{
  TwoCNFFormula formula{graph.GetVertexCount()};
  for (const auto& [vertex_u, vertex_v] : graph.GetEdges()) {
    for (size_t variable_u = 0; variable_u < 2; ++variable_u) {
      for (size_t variable_v = 0; variable_v < 2; ++variable_v) {
        if (available_colors[vertex_u][variable_u] == available_colors[vertex_v][variable_v]) {
          formula.AddDisjunction(
              Literal{vertex_u, variable_u},
              Literal{vertex_v, variable_v});
        }
      }
    }
  }
  return formula;
}

Coloring Randomized3ColoringAlgo::GetColoringFromSatisfyingAssignment(
        const std::vector<AvailableColors>& available_colors,
        const SatisfyingAssignment& satisfying_assignment) const
{
  Coloring coloring;
  for (Vertex vertex = 0; vertex < available_colors.size(); ++vertex) {
    coloring.emplace_back(available_colors[vertex][satisfying_assignment[vertex]]);
  }
  return coloring;
}

std::vector<Randomized3ColoringAlgo::AvailableColors>
Randomized3ColoringAlgo::GetRandomAvailableColors(size_t vertex_count) {
  std::vector<AvailableColors> available_colors;
  for (size_t index = 0; index < vertex_count; ++index) {
    available_colors.emplace_back();
    size_t banned_color = random_engine_.GenerateRandomValue(1, 3);
    for (Color color = 0; color < 3; ++color) {
      if (color != banned_color) {
        available_colors.back().emplace_back(color);
      }
    }
  }
  return available_colors;
}