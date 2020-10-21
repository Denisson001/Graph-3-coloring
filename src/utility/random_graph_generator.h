#pragma once

#include <base/graph.h>
#include <base/coloring.h>

class RandomGraphGenerator {
public:
  Graph Generate3ColoringGraph(size_t vertex_count);

private:
  Coloring Generate3Coloring(size_t vertex_count);

  bool FlipCoin(double coin_probability);
};