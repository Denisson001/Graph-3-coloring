#pragma once

#include <base/graph.h>
#include <base/coloring.h>

class GraphColoringChecker {
public:
  bool CheckColoring(const Graph& graph, const Coloring& coloring);
};