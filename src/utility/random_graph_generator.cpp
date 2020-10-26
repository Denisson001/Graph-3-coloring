#include "random_graph_generator.h"

UndirectedGraph RandomGraphGenerator::Generate3ColoringGraph(size_t vertex_count) {
  UndirectedGraph graph{vertex_count};
  const auto coloring = Generate3Coloring(vertex_count);
  for (Vertex u = 0; u < vertex_count; ++u) {
    for (Vertex v = u + 1; v < vertex_count; ++v) {
      static const double coin_probability = 0.7;
      if (coloring[u] != coloring[v] &&
          FlipCoin(coin_probability)) {
        graph.AddEdge({u, v});
      }
    }
  }
  return graph;
}

Coloring RandomGraphGenerator::Generate3Coloring(size_t vertex_count) {
  Coloring coloring;
  for (Vertex index = 0; index < vertex_count; ++index) {
    coloring.push_back(rand() % 3 + 1); // !!!!!!!!!!!!!!!!!!!!11
  }
  return coloring;
}

bool RandomGraphGenerator::FlipCoin(double coin_probability) {
  return rand() % 100 < 100 * coin_probability;
}