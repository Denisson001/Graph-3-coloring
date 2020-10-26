#include "graph.h"

Graph::Graph(size_t vertex_count)
    : vertex_count_(vertex_count),
      adjacent_vertices_(vertex_count) {}

size_t Graph::GetVertexCount() const {
  return vertex_count_;
}

const Edges& Graph::GetEdges() const {
  return edges_;
}

const std::vector<Vertex>& Graph::GetAdjacentVertices(Vertex vertex) const {
  return adjacent_vertices_[vertex];
}

DirectedGraph::DirectedGraph(size_t vertex_count)
    : Graph(vertex_count) {}

void DirectedGraph::AddEdge(Edge edge) {
  adjacent_vertices_[edge.first].emplace_back(edge.second);
  edges_.emplace_back(std::move(edge));
}


UndirectedGraph::UndirectedGraph(size_t vertex_count)
    : Graph(vertex_count) {}

void UndirectedGraph::AddEdge(Edge edge) {
  adjacent_vertices_[edge.first].emplace_back(edge.second);
  adjacent_vertices_[edge.second].emplace_back(edge.first);
  edges_.emplace_back(std::move(edge));
}