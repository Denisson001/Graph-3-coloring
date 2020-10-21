#include "graph.h"

Graph::Graph(size_t vertex_count)
    : vertex_count_(vertex_count) {}

void Graph::AddEdge(Edge edge) {
  edges_.emplace_back(std::move(edge));
}

size_t Graph::GetVertexCount() const {
  return vertex_count_;
}

const Edges& Graph::GetEdges() const {
  return edges_;
}