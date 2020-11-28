#pragma once

#include <base/coloring.h>
#include <base/graph.h>
#include <utility/random_values_generator.h>

class GraphGenerator {
public:
  std::vector<UndirectedGraph> GenerateAllGraphs(size_t vertex_count);

protected:
  std::vector<Edge> GetAllEdges(size_t vertex_count) const;
};


class RandomGraphGenerator : public GraphGenerator {
public:
  RandomGraphGenerator();

  UndirectedGraph Generate3ColoringGraph(size_t vertex_count, double coin_probability = 0.7);

private:
  Coloring Generate3Coloring(size_t vertex_count);

private:
  RandomValuesGenerator random_engine_;
};