#include "random_graph_generator.h"

RandomGraphGenerator::RandomGraphGenerator()
    : random_engine_("RandomGraphGenerator") {}

UndirectedGraph RandomGraphGenerator::Generate3ColoringGraph(size_t vertex_count, double coin_probability) {
  UndirectedGraph graph{vertex_count};
  const auto coloring = Generate3Coloring(vertex_count);
  for (Vertex u = 0; u < vertex_count; ++u) {
    for (Vertex v = u + 1; v < vertex_count; ++v) {
      if (coloring[u] != coloring[v] &&
          random_engine_.FlipCoin(coin_probability)) {
        graph.AddEdge({u, v});
      }
    }
  }
  return graph;
}

Coloring RandomGraphGenerator::Generate3Coloring(size_t vertex_count) {
  Coloring coloring;
  for (Vertex index = 0; index < vertex_count; ++index) {
    coloring.push_back(random_engine_.GenerateRandomValue(1, 3));
  }
  return coloring;
}