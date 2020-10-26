#include "randomized_algo.h"

Coloring Randomized3ColoringAlgo::GetColoring(const Graph& graph) {
  for (size_t iter_num = 0; ; ++iter_num) {
    const auto coloring = DoIteration(graph);
    if (coloring.has_value()) {
      return *coloring;
    }
  }
}

std::optional<Coloring> Randomized3ColoringAlgo::DoIteration(const Graph& graph) {
  const auto available_colors = GetRandomAvailableColors(graph.GetVertexCount());
  const auto two_cnf_formula = MakeTwoCNFFormula(graph, available_colors);
  const auto satisfying_assignment = two_SAT_solver_.GetSatisfyingAssignment(std::move(two_cnf_formula));
  if (!satisfying_assignment.has_value()) {
    return {};
  }
  return GetColoringFromSatisfyingAssignment(available_colors, *satisfying_assignment);
}

TwoCNFFormula Randomized3ColoringAlgo::MakeTwoCNFFormula(
    const Graph& graph,
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
Randomized3ColoringAlgo::GetRandomAvailableColors(size_t vertex_count) const {
  std::vector<AvailableColors> available_colors;
  for (size_t index = 0; index < vertex_count; ++index) {
    available_colors.emplace_back();
    size_t banned_color = rand() % 3; // !!!!!!!!!!!
    for (Color color = 0; color < 3; ++color) {
      if (color != banned_color) {
        available_colors.back().emplace_back(color);
      }
    }
  }
  return available_colors;
}