#pragma once

#include <cstdlib>
#include <vector>

using Vertex = size_t;
using Edge = std::pair<Vertex, Vertex>;
using Edges = std::vector<Edge>;

class Graph {
public:
  explicit Graph(size_t vertex_count);
  void AddEdge(Edge edge);

  size_t GetVertexCount() const;
  const Edges& GetEdges() const;

private:
  size_t vertex_count_;
  Edges edges_;
};