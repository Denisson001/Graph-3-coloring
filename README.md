# Алгоритмы поиска правильной раскраски графа в 3 цвета

## Algorithms

Реализованы два вероятностных алгоритма поиска правильной раскраски графа в 3 цвета.

- `/src/algo/randomized_3coloring_algo.*` имеет асимптотическую сложность O(1.5^n n^2), где n - число вершин в графе.
 
- `/src/algo/randomized_CSP_solver.*` имеет асимптотическую сложность O(1.415^n).

## Usage example

- Собрать проект

```shell script
mkdir build
cd build
cmake ../src
make
```

- Запустить бенчмарк. В формате `.csv` будут сохранены результаты запуска.

```shell script
./graph_coloring_benchmark Randomized3ColoringAlgoBenchmark
```

- Полный список бенчмарков

  - `Randomized3ColoringAlgoBenchmark`
  - `Randomized3ColoringAlgoAllSmallGraphsBenchmark`
  - `RandomizedCSPSolverBenchmark`
  - `RandomizedCSPSolverOneGraphIterDistributionBenchmark`
  
- Описание бенчмарков можно найти в тексте к проекту, а реализация находится в `/src/benchmark/benchmark_driver.cpp`

## Project structure

- `/src/algo` - реализация алгоритмов
- `/src/benchmark` - код для запуска бенчмарков
- `/src/base` - структуры для работы с основными математическими объектами
- `/src/utility` - вспомогательные техничекие реализации
- `/src/math_utility` - вспомогательные алгоритмы
- `/srctipts` - скрипты для генерации графиков
- `/data` - результаты бенчмарков
- `/charts` - используемые в тексте гарфики
