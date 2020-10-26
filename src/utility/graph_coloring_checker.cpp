#include "graph_coloring_checker.h"

bool GraphColoringChecker::CheckColoring(const Graph& graph, const Coloring& coloring) {
  for (const auto& [u, v] : graph.GetEdges()) {
    if (coloring[u] == coloring[v]) {
      return false;
    }
  }
  return true;
}