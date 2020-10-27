#pragma once

#include <base/coloring.h>
#include <base/graph.h>
#include <utility/random_values_generator.h>

class RandomGraphGenerator {
public:
  RandomGraphGenerator();

  UndirectedGraph Generate3ColoringGraph(size_t vertex_count);

private:
  Coloring Generate3Coloring(size_t vertex_count);

private:
  RandomValuesGenerator random_engine_;
};