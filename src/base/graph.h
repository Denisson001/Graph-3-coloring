#pragma once

#include <cstdlib>
#include <vector>

using Vertex = size_t;
using Edge = std::pair<Vertex, Vertex>;
using Edges = std::vector<Edge>;

class Graph {
public:
  explicit Graph(size_t vertex_count);
  virtual ~Graph() {}

  virtual void AddEdge(Edge edge) = 0;

  size_t GetVertexCount() const;
  const Edges& GetEdges() const;
  const std::vector<Vertex>& GetAdjacentVertices(Vertex vertex) const;

protected:
  size_t vertex_count_;
  Edges edges_;
  std::vector<std::vector<Vertex>> adjacent_vertices_;
};


class DirectedGraph : public Graph {
public:
  DirectedGraph(size_t vertex_count);
  void AddEdge(Edge edge);
};


class UndirectedGraph : public Graph {
public:
  UndirectedGraph(size_t vertex_count);
  void AddEdge(Edge edge);
};