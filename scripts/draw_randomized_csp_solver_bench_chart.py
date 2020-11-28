#!/usr/bin/python3

import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

grid = np.arange(80) + 1

sns.set(palette="Set2")
plt.figure(figsize=(9, 6))
with sns.axes_style("darkgrid"):
    for index, probability in enumerate([0.1, 0.125, 0.15, 0.175, 0.2]):
        data_file_name = sys.argv[index + 1]
        df = pd.read_csv(data_file_name, header=None)
        sample = pd.DataFrame(df.to_numpy().T.mean(axis=1)).rolling(window=5).mean().to_numpy()
        plt.plot(grid, sample, label=fr"$p={probability}$", lw=2)

plt.tick_params(axis='both', labelsize=8)
plt.locator_params(nbins=20)
plt.xlim(50, 80)
plt.xlabel("Мощность множества вершин графа", fontsize=9)
plt.ylabel("Количество итераций алгоритма", fontsize=9)
# plt.title(
#     "Количество итераций алгоритма в зависимости от количества вершин в графе",
#     y=1.05, fontsize=13)
plt.grid(ls=":")
plt.legend(fontsize=9)

plt.savefig("randomized_csp_solver_bench_chart.png")
