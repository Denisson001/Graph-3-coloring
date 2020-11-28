#pragma once

#include <base/graph.h>
#include <base/coloring.h>

#include <vector>
#include <optional>

class Graph3ColoringAlgo {
public:
  virtual std::optional<Coloring> GetColoring(const UndirectedGraph& graph) = 0;
  virtual ~Graph3ColoringAlgo() {}
  virtual size_t GetLastRunIterationCount() const { return 0; }
};