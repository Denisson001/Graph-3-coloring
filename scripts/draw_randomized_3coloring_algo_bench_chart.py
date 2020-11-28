#!/usr/bin/python3

import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

grid = np.arange(30) + 1

sns.set(palette="Set2")
plt.figure(figsize=(9, 6))
with sns.axes_style("darkgrid"):
    plt.plot(grid, (1.5 ** grid), label="Мат. ож. числа итераций", linestyle="dashed", lw=2)
    for index, probability in enumerate([0.3, 0.5, 0.7, 0.9]):
        data_file_name = sys.argv[index + 1]
        df = pd.read_csv(data_file_name, header=None)
        plt.plot(grid, df.to_numpy().T.mean(axis=1), label=fr"$p={probability}$", lw=2)

plt.tick_params(axis='both', labelsize=8)
plt.ticklabel_format(axis="y", style="sci", scilimits=(0, 0))
plt.locator_params("x", nbins=10)
plt.xlim(20, 30)
plt.xlabel("Мощность множества вершин графа", fontsize=9)
plt.ylabel("Количество итераций алгоритма", fontsize=9)
# plt.title(
#     "Количество итераций алгоритма в зависимости от количества вершин в графе",
#     y=1.05, fontsize=13)
plt.grid(ls=":")
plt.legend(fontsize=9)

plt.savefig("randomized_3coloring_algo_bench_chart.png")
