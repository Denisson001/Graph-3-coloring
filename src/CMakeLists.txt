cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2 -Wall")

set(PROJECT_NAME graph_coloring_benchmark)
project(${PROJECT_NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
link_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(algo)
add_subdirectory(base)
add_subdirectory(benchmark)
add_subdirectory(utility)

add_executable(${PROJECT_NAME}
  ${ALGO_SRC}
  ${BASE_SRC}
  ${UTILITY_SRC}
  ${BENCHMARK_SRC}
)