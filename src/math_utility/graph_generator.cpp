#include "graph_generator.h"

RandomGraphGenerator::RandomGraphGenerator()
    : random_engine_("RandomGraphGenerator") {}

UndirectedGraph RandomGraphGenerator::Generate3ColoringGraph(size_t vertex_count, double coin_probability) {
  UndirectedGraph graph{vertex_count};
  const auto coloring = Generate3Coloring(vertex_count);
  for (const auto [u, v] : GetAllEdges(vertex_count)) {
    if (coloring[u] != coloring[v] &&
        random_engine_.FlipCoin(coin_probability)) {
      graph.AddEdge(Edge{u, v});
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

std::vector<UndirectedGraph> GraphGenerator::GenerateAllGraphs(size_t vertex_count) {
  std::vector<UndirectedGraph> graphs;
  const auto edges = GetAllEdges(vertex_count);
  for (size_t edge_mask = 0; edge_mask < (1 << edges.size()); ++edge_mask) {
    UndirectedGraph graph{vertex_count};
    for (size_t edge_id = 0; edge_id < edges.size(); ++edge_id) {
      if ((edge_mask >> edge_id) & 1) {
        graph.AddEdge(edges[edge_id]);
      }
    }
    graphs.emplace_back(std::move(graph));
  }
  return graphs;
}

std::vector<Edge> GraphGenerator::GetAllEdges(size_t vertex_count) const {
  std::vector<Edge> edges;
  for (Vertex u = 0; u < vertex_count; ++u) {
    for (Vertex v = u + 1; v < vertex_count; ++v) {
      edges.emplace_back(Edge{u, v});
    }
  }
  return edges;
}