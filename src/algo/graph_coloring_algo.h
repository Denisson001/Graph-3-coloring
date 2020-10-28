#pragma once

#include <base/graph.h>
#include <base/coloring.h>

#include <vector>

class Graph3ColoringAlgo {
public:
  virtual Coloring GetColoring(const UndirectedGraph& graph) = 0;
  virtual ~Graph3ColoringAlgo() {}
};